#pragma once

#include "Behaviour.h"
#include "Renderer.h"

class RendererEnabler :
    public Behaviour
{
    Renderer* renderer;
public:
    RendererEnabler(GameObject* obj);

    void update() override {
        if (input->getMouseButtonDown(0)) {
            renderer->setEnabled(!renderer->getEnabled());
        }
    }
};

