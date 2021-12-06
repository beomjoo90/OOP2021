#include "RendererEnabler.h"
#include "GameObject.h"
#include "Renderer.h"

RendererEnabler::RendererEnabler(GameObject* obj) : Behaviour(obj) {
   renderer->setEnabled(false);
}
