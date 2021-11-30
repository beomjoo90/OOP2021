#pragma once
#include <string>

using namespace std;
#include "GameObject.h"

class GameObject;
class Transform;

class Component
{	
	string		tag;

protected:
	GameObject* getParent() const { return gameObject->parent; }
	Transform* getTransform() const { return transform;  }
	GameObject* gameObject;
	Transform*	transform;
	
public:
	Component(GameObject* gameObject);

	virtual ~Component() {}

	friend class GameObject;

};

