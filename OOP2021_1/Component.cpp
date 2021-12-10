#include "GameObject.h"
#include "Component.h"


Component::Component(GameObject* gameObject)
	: gameObject(gameObject), tag(gameObject->tag), transform(gameObject->transform), input(Input::GetInstance() ), enabled(true)
{
}

GameObject* Component::getParent() const { return gameObject->parent; }
Transform*	Component::getTransform() const { return transform; }

void	Component::destroy(GameObject* obj) {
	if (!obj) return;
	obj->destroy();
}

void	Component::destroy(Component* component) {
	gameObject->removeComponent(component);
}