#pragma once
#include <iostream>
#include "GameObject.h"
#include "Position.h"
#include "Renderer.h"

using namespace std;

class PanelRenderer :
    public Renderer
{
    string title;

public:
    PanelRenderer(GameObject* gameObject) 
        : Renderer(gameObject, nullptr, gameObject->getRenderer()->getDimension() + Position::ones) {}

    void setTitle(const string& title) { this->title = title; }

    void draw() override {
        screen->drawRectangle({ worldPos.x-1, worldPos.y-1 }, { dim.x, dim.y });
        if (title.length() == 0) return;
        screen->draw({ worldPos.x + 1, worldPos.y-1 }, title.c_str());
    }
};

