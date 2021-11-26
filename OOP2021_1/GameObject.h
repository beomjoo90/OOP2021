#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Screen.h"
#include "Transform.h"
#include "Renderer.h"
#include "Input.h"

using namespace std;



class GameObject
{
private:
	string				tag;
	string				name;

	Transform*			transform; // should not be nullptr
	Renderer*			renderer;  // should not be nullptr

	bool				active;	// indicator whether it is being served by the update/render logic
								// if "false", it should not be served by the game engine.
	bool				paused;
	bool				hidden;

	friend class Component;

protected:

	GameObject* parent;
	vector<GameObject*> children;
	vector<Component*>	components;

	void add(GameObject* child) { children.push_back(child); }

public:

	GameObject(string name = "unknown", string tag = "unknown", 
		const char* face = nullptr, const Dimension& dim = { 0,0 }, // renderer
		const Position& pos = Position::zeros, const Position& rot = Position::zeros,  // transform
		GameObject* parent = nullptr)
		: transform(new Transform(this, pos, rot)), renderer(new Renderer(this, face, dim)),
		parent(parent), tag(tag), name(name),
		active(true), paused(false), hidden(false)
	{
		if (parent) parent->add(this);
		components.push_back(transform);
		components.push_back(renderer);
	}

	virtual ~GameObject()
	{
		while (children.size() > 0)
		{
			auto child = children.back();
			children.pop_back();
			delete child;
		}
		while (components.size() > 0)
		{
			auto component = components.back();
			components.pop_back();
			delete component;
		}
	}

	Transform* getTransform() const { return transform; }


	

	// utility functions

	//int pos2Offset(const Position& pos) const { return pos.y * dim.x + pos.x; }
	//Position offset2Pos(int offset) const { return Position{ offset % dim.x, offset / dim.x }; }
		

	virtual bool isActive() const { return active; }
	void setActive(bool flag = true) { this->active = flag; }	

	void setParent(GameObject* parent) {
		this->parent = parent;
		if (this->parent) {
			parent->add(this);
			transform->setParentWorldPos(parent->transform->local2World());
		}
		else {
			/* TODO */
			// i don't know how to add root parent's children
			transform->setParentWorldPos(Position::zeros);
		}
		for (auto child : children) child->updatePos(true);
	}

	

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

