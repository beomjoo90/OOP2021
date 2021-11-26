#pragma once
#include <string>

using namespace std;
#include "GameObject.h"

class GameObject;
class Transform;

class Component
{
	GameObject* gameObject;
	string		tag;
	Transform*	transform;

protected:
	GameObject* getParent() const { return gameObject->parent; }
	Transform* getTransform() const { return transform;  }
	
public:
	Component(GameObject* gameObject);

	virtual ~Component() {}

	friend class GameObject;

};

