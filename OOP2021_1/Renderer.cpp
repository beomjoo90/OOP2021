#include "Renderer.h"
#include "Transform.h"
#include "GameObject.h"

void Renderer::draw() { 
    auto pos = transform->local2World();
	if (!shape) return;  screen->draw(transform->local2World(), shape, dim); 
}

void Renderer::render() {
    if (enabled == false) return;
    if (gameObject->getName() == "nextBlock") {
        Borland::gotoxy(0, 37);
        std::cout << dim << transform->local2World() << getParent()->getName() ;
        screen->draw(transform->local2World(), shape, dim);
    }
    draw();
}