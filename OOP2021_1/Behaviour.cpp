#include "GameObject.h"
#include "Behaviour.h"

Behaviour::Behaviour(GameObject* gameObject) 
	: Component(gameObject), input(Input::GetInstance()) {
	renderer = gameObject->getComponent<Renderer>();
}