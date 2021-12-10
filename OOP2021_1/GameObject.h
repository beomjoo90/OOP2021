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
	bool				alive;	// if "false", it should be removed from the system
	bool				paused;
	bool				hidden;

	friend class Component;

protected:

	GameObject* parent;
	vector<GameObject*> children;
	vector<Component*>	components;	

	void internalRemove() {
		for (auto it = children.begin(); it != children.end(); )
		{
			auto child = *it;
			if (child->isAlive() == false) {
				child->internalRemove();
				it = children.erase(it);
				delete child;
			}
			else it++;
		}
	}

	void internalStart() {
		if (alive == false || active == false || paused == true) return;

		for (auto component : components) component->start();
		for (auto child : children) child->internalStart();
	}

	void internalUpdate() {
		if (alive == false || active == false || paused == true) return;

		for (auto component : components) component->update();
		for (auto child : children) child->internalUpdate();
	}

	void internalRender(const Position& parentWorldPos, bool dirty=false);

	void destroy() { this->alive = false; }

public:

	GameObject(GameObject* parent = nullptr, string name = "unknown", string tag = "unknown",
		const char* face = nullptr, const Dimension& dim = { 0,0 },
		const Position& pos = Position::zeros, const Position& rot = Position::zeros);

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

	void addChild(GameObject* child);
	void removeChild(GameObject* child);

	virtual bool isActive() const { return active; }
	void setActive(bool flag = true) { this->active = flag; }	

	bool isAlive() const { return alive; }
	void setAlive(bool flag = true) { this->alive = flag; }

	
	Transform* getTransform() const { return transform; }
	Renderer* getRenderer() const { return renderer; }

	void setParent(GameObject* parent) {
		if (this->parent) this->parent->removeChild(this);		
		this->parent = parent;
		if (parent) parent->addChild(this);
		transform->setDirty(true);
	}

	GameObject* getParent() const { return parent; }

	const string& getName() const { return name;  }
	void setName(const string& name) { this->name = name; }
	
	virtual void update() {}

	void setFreeze(bool paused = true) {
		this->paused = paused;
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

	void removeComponent(Component* removed) {
		for (auto it = components.begin(); it != components.end(); ) {
			auto component = *it;
			if (component != removed) {
				it++;
				continue;
			}
			it = components.erase(it);
			delete component;
		}
	}
};

