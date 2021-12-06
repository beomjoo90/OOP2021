#include "GameObject.h"
#include "Renderer.h"


GameObject::GameObject(string name, string tag,
	const char* face, const Dimension& dim, // renderer
	const Position& pos, const Position& rot,  // transform
	GameObject* parent)
	: tag(tag), name(name), active(true), paused(false), hidden(false), parent(parent), 
	transform(nullptr), renderer(nullptr)
{	
	if (parent) parent->add(this);

	transform = new Transform(this, pos, rot);
	renderer = new Renderer(this, face, dim);

	components.push_back(transform);
	components.push_back(renderer);
}

void GameObject::internalRender() {
	if (active == false) return;

	for (auto component : components) {
		auto render = dynamic_cast<Renderer*>(component);
		if (render == nullptr) continue;
		render->render();
	}
	for (auto child : children) child->internalRender();
}