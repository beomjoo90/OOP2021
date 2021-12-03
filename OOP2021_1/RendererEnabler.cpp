#include "RendererEnabler.h"
#include "GameObject.h"
#include "Renderer.h"

RendererEnabler::RendererEnabler(GameObject* obj) : Behaviour(obj) {
    renderer = gameObject->getComponent<Renderer>();
    renderer->setEnabled(false);
}
