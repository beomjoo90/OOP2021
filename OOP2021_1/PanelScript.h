#pragma once
#include <iostream>
#include "GameObject.h"
#include "Position.h"
#include "Renderer.h"

using namespace std;

class PanelScript :
    public Renderer
{
    string title;

public:
    PanelScript(GameObject* gameObject) 
        : Renderer(gameObject, nullptr, gameObject->getRenderer()->getDimension() + Position::ones) {}

    void setTitle(const string& title) { this->title = title; }

    void draw() override {
        Position pos = transform->local2World();
        screen->drawRectangle({ pos.x-1, pos.y-1 }, { dim.x, dim.y });
        if (title.length() == 0) return;
        screen->draw({ pos.x + 1, pos.y-1 }, title.c_str());
    }
};

