#pragma once
#include <string>
#include "Input.h"

using namespace std;


class GameObject;
class Transform;

class Component
{	
	string		tag;

protected:
	GameObject*	getParent() const;
	Transform*	getTransform() const;
	GameObject* gameObject;
	Transform*	transform;
	Input*		input;

	bool		enabled;

	
	
public:
	Component(GameObject* gameObject);

	virtual ~Component() {}

	friend class GameObject;

	virtual void update() {}

	void	setEnabled(bool enabled = true) { this->enabled = enabled; }
	bool	getEnabled() const { return enabled; }

};

