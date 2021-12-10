#pragma once
#include <string>
#include "Input.h"

using namespace std;


class GameObject;
class Transform;

class Component
{	
	string		tag;

	friend class GameObject;

protected:
	GameObject*	getParent() const;
	Transform*	getTransform() const;

	GameObject* gameObject;
	Transform*	transform;
	Input*		input;

	bool		enabled;

	void	destroy(GameObject* obj);
	void	destroy(Component* component);

	void	setEnabled(bool enabled = true) { enabled = enabled; }
	bool	getEnabled() const { return enabled; }
	
public:
	Component(GameObject* gameObject);

	virtual ~Component() {}

	virtual void start() {}

	virtual void update() {}

	
};

