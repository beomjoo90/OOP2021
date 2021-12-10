#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"

GameObject::GameObject(GameObject* parent, string name, string tag,
	const char* face, const Dimension& dim, 
	const Position& pos, const Position& rot) 
	: tag(tag), name(name), active(true), paused(false), hidden(false), parent(parent),
	transform(nullptr), renderer(nullptr), alive(true)
{	
	if (parent) parent->addChild(this);

	transform = new Transform(this, pos, rot);
	renderer = new Renderer(this, face, dim);

	components.push_back(transform);
	components.push_back(renderer);
}

void GameObject::internalRender(const Position& parentWorldPos, bool dirty) {
	if (alive == false || active == false) return;
	dirty = dirty || transform->getDirty();
	transform->setDirty(dirty);
	for (auto component : components) {
		auto compRenderer = dynamic_cast<Renderer*>(component);
		if (compRenderer == nullptr) continue;
		compRenderer->render(parentWorldPos, dirty);
	}
	transform->setDirty(false);
	for (auto child : children) child->internalRender(transform->local2World(parentWorldPos), dirty);
}


void GameObject::addChild(GameObject* child) {
	if (!child) return;
	children.push_back(child);
}

void GameObject::removeChild(GameObject* child) {
	if (!child) return;
	auto it = find(children.begin(), children.end(), child);
	if (it == children.end()) return;
	children.erase(it);
}