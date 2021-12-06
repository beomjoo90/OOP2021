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

public:

	GameObject(string name = "unknown", string tag = "unknown",
		const char* face = nullptr, const Dimension& dim = { 0,0 }, // renderer
		const Position& pos = Position::zeros, const Position& rot = Position::zeros,  // transform
		GameObject* parent = nullptr);

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
	Renderer* getRenderer() const { return renderer; }
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

	const string& getName() const { return name;  }
	void setName(const string& name) { this->name = name; }
	
	void update() {}

	void setFreeze(bool paused = true) {
		this->paused = paused;
	}

	void internalUpdate() {
		if (active == false || paused == true) return;

		for (auto component : components) component->update();
		updatePos(transform->getDirty());
		for (auto child : children) child->internalUpdate();

	}

	void internalRemove() {
		for (auto child : children) child->internalRemove();
		children.erase(
			std::remove_if(children.begin(), children.end(),
				[](auto child) { return child->isActive() == false; }),
			children.end());
	}

	void internalRender();

	void remove(GameObject* child) {
		if (!child) return;
		auto it = find(children.begin(), children.end(), child);
		if (it == children.end()) return;
		children.erase(it);
	}

	template<typename T>
	void addComponent() {
		T* newComponent = new T(this);
		components.push_back(newComponent);
	}

	template<typename T>
	T* getComponent() {
		for (auto component : components) {
			auto result = dynamic_cast<T*>(component);
			if (result == nullptr) continue;
			return result;
		}
		return nullptr;
	}

	template<typename T>
	T* getOrAddComponent() {
		auto comp = getComponent<T>();
		if (comp) return comp;
		addComponent<T>();
		return getComponent<T>();
	}

	template<typename T>
	void removeComponent() {
		for (auto it = components.begin(); it != components.end(); )
		{
			auto component = *it;
			if (!dynamic_cast<T*>(component)) {
				it++;
				continue;
			}
			it = components.erase(it);
			delete component;
		}
	}
	
};

