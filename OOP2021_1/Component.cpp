#include "GameObject.h"
#include "Component.h"

Component::Component(GameObject* gameObject)
	: gameObject(gameObject), tag(gameObject->tag), transform(gameObject->transform)
{
}