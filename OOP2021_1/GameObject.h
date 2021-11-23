#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Screen.h"
#include "Input.h"

using namespace std;

class Transform;
class Component;

class GameObject
{
private:
	string				tag;
	string				name;
	Transform*			transform;

	bool				active;	// indicator whether it is being served by the update/render logic
								// if "false", it should not be served by the game engine.
	bool				paused;
	bool				hiden;

protected:

	GameObject* parent;
	vector<GameObject*> children;
	vector<Component*>	components;

public:

	GameObject(const char* face, const Position& pos, const Dimension& dim, GameObject* parent = nullptr)
		: 
		: pos(pos), dim(dim), capacity((size_t)dim.x* dim.y),
		shape{ new char[(size_t)dim.x * dim.y] }, active(true),
		dirty(false), paused(false), 
		parent(parent), parentWorldPos(parent ? parent->local2Screen() : Position::zeros)
	{
		if (parent) parent->add(this);
		setShape(face);
	}

	virtual ~GameObject()
	{
		while (children.size() > 0)
		{
			auto child = children.back();
			children.pop_back();
			delete child;
		}
		if (shape != nullptr)
			delete[] shape;
		shape = nullptr;
	}

	virtual void move(const Position& offset) {}

	// utility functions

	int pos2Offset(const Position& pos) const { return pos.y * dim.x + pos.x; }
	Position offset2Pos(int offset) const { return Position{ offset % dim.x, offset / dim.x }; }

	Position getPos() const { return pos; }
	void setPos(const Position& pos) { this->pos = pos; dirty = true; }
	void setPos(int x, int y) { setPos(Position{ x, y }); }

	virtual bool isActive() const { return active; }
	void setActive(bool flag = true) { this->active = flag; }

	int getWidth() const { return dim.x; }
	int getHeight() const { return dim.y; }

	int getCapacity() const { return capacity; }

	void setParent(GameObject* parent) {
		this->parent = parent;
		parent->add(this);
		setParentWorldPos(parent ? parent->local2Screen() : Position::zeros);
		for (auto child : children) child->updatePos(true);
	}

	Position screen2Local(const Position& screenPos) const {
		return screenPos - local2Screen();
	}

	Position local2Screen(const Position& pos ) const {
		return parentWorldPos + pos;
	}

	Position local2Screen() const { return local2Screen(getPos()); }

	const Dimension& getDimension() const { return dim; } // why getDimension returns reference value?
	void setDimension(const Dimension& dim) { this->dim = dim; }	
	
	const char* getShape() const { return shape; }
	void setShape(const char* face) {
		memset(shape, ' ', sizeof(char) * capacity);
		if (face == nullptr || face[0] == '\0') {
			return;
		}
		strncpy(shape, face, min(capacity, strlen(face)));
	}
	void setShape(char shape, int offset) {
		if (offset < 0 || offset >= capacity) return;
		this->shape[offset] = shape;
	}
	void setShape(char shape, const Position& pos) { setShape(shape, pos2Offset(pos)); }

	void updatePos(bool dirty = false) {
		bool inheritedDirty = dirty;
		if (inheritedDirty == false) {
			if (this->dirty == true)
				inheritedDirty = true;
		} else {
			setParentWorldPos( parent ? parent->local2Screen() : Position::zeros );
		}

		for (auto child : children) child->updatePos(inheritedDirty); /* update all children's loc */
		this->dirty = false; // all its children were updated successfully.
	}

	void internalDraw() {
		if (active == false) return;
		draw();
		for (auto child : children) child->internalDraw();
	}
	virtual void draw() { screen->draw(local2Screen(), shape, dim); }
	
	void internalUpdate() {
		if (active == false || paused == true) return;
		update();
		for (auto child : children) child->internalUpdate();
	}
	virtual void update() {}


	void setFreeze(bool paused = true) {
		this->paused = paused;
	}

	void add(GameObject* child) {
		if (!child) return;
		children.push_back(child);
	}

	void remove(GameObject* child) {
		if (!child) return;
		auto it = find(children.begin(), children.end(), child);
		if (it == children.end()) return;
		children.erase(it);
	}
};

