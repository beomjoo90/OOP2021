#include "GameObject.h"


GameObject::GameObject(string name, string tag,
	const char* face, const Dimension& dim, // renderer
	const Position& pos, const Position& rot,  // transform
	GameObject* parent)
	: transform(new Transform(this, pos, rot)), renderer(new Renderer(this, face, dim)),
	parent(parent), tag(tag), name(name),
	active(true), paused(false), hidden(false)
{
	if (parent) parent->add(this);
	components.push_back(transform);
	components.push_back(renderer);
}

void GameObject::internalRender() {
	if (active == false) return;

	renderer->render();
	for (auto child : children) child->internalRender();
}