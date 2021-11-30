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

	void updatePos(bool dirty) {
		transform->setDirty(dirty || transform->getDirty());
		if (transform->getDirty() == false) return;

		// it means transform marked as "dirty".
		if (parent == nullptr) {
			/* TODO */
			// i don't know how to add root parent's children
			transform->setParentWorldPos(Position::zeros);
		}
		else {
			transform->setParentWorldPos(parent->transform->local2World());
		}
		for (auto child : children) {
			child->updatePos(true);
		}
		transform->setDirty(false);
	}

	void internalUpdate() {
		if (active == false || paused == true) return;
		update();
		for (auto child : children) child->internalUpdate();
	}

	void internalDraw() {
		if (active == false) return;
		renderer->draw();
		for (auto child : children) child->internalDraw();
	}

protected:

	GameObject* parent;
	vector<GameObject*> children;
	vector<Component*>	components;

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

	// utility functions

	//int pos2Offset(const Position& pos) const { return pos.y * dim.x + pos.x; }
	//Position offset2Pos(int offset) const { return Position{ offset % dim.x, offset / dim.x }; }

	virtual bool isActive() const { return active; }
	void setActive(bool flag = true) { this->active = flag; }	

	
	Transform* getTransform() const { return transform; }
	void setParent(GameObject* parent) {
		if (this->parent) {
			this->parent->remove(this);
		}
		/* TODO */
		// if previous parent is a root meaning parent is nullptr, 
		// we need extra treatment on the root node.

		this->parent = parent;
		if (parent) {
			parent->add(this);
		}
		updatePos(true);
	}
	GameObject* getParent() const { return parent; }
	
	void update() {}

	void setFreeze(bool paused = true) {
		this->paused = paused;
	}
};

