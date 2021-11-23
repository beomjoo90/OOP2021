#pragma once
#include <string>

using namespace std;

class GameObject;
class Transform;

class Component
{
	GameObject* gameObject;
	string*		tag;
	Transform*	transform;
	
public:
	Component(GameObject* gameObject) 
		: gameObject(gameObject), tag(gameObject->tag), transform(gameObject->transform)
	{
		
	}

	virtual ~Component() {}

	friend class GameObject;

};

