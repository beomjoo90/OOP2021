#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Screen.h"
#include "Input.h"

using namespace std;

class GameObject
{
private:
	char*				shape;
	Position			pos;		// "relative" position to its parent game object.
	
	int					capacity;	// allocated memory size of "shape"
	bool				dirty;		// if its pos is updated, it should be marked as dirty
									// to make sure all the locations of its children game objects
									// are updated accordingly.
	Position			parentWorldPos;	// "absolute" position of its parent game object to the screen 
	bool				active;	// indicator whether it is being served by the update/render logic
									// if "false", it should not be served by the game engine.

protected:

	Screen*				screen;
	Input*				input;

	vector<GameObject*> children;
	GameObject*			parent;
	Dimension			dim;

	void setParentWorldPos(const Position& parentWorldPos) { this->parentWorldPos = parentWorldPos; }

	void add(GameObject* child) {
		if (!child) return;
		children.push_back(child);
	}

public:

	GameObject(const char* face, const Position& pos, const Dimension& dim, GameObject* parent = nullptr)
		: pos(pos), dim(dim), capacity((size_t)dim.x* dim.y),
		shape{ new char[(size_t)dim.x * dim.y] }, active(true),
		screen(Screen::GetInstance()), input(Input::GetInstance()), dirty(false), 
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
		setParentWorldPos(parent ? parent->local2Screen() : Position::zeros);
		for (auto child : children) child->updatePos(true);
	}

	Position screen2local(const Position& screenPos) const {
		Position pos = local2Screen();
		return Position{ screenPos.x - pos.x, screenPos.y - pos.y };
	}

	Position local2Screen(const Position& pos) const {
		return { parentWorldPos.x + pos.x, parentWorldPos.y + pos.y };
	}

	Position local2Screen() const { return local2Screen(getPos()); }

	const Dimension& getDimension() const { return dim; } // why getDimension returns reference value?
	void setDimension(const Dimension& dim) { this->dim = dim; }	
	
	const char* getShape() const { return shape; }
	void setShape(const char* face) {
		if (face == nullptr || face[0] == '\0') {
			memset(shape, ' ', sizeof(char) * capacity);
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
		if (active == false) return;
		update();
		for (auto child : children) child->internalUpdate();
	}
	virtual void update() {}

};

