#include "Renderer.h"
#include "Transform.h"

void Renderer::draw() { 
	if (!shape) return;  screen->draw(transform->local2World(), shape, dim); 
}