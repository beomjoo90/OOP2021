#pragma once
#include "Screen.h"
#include "Component.h"

class Renderer :
    public Component
{
    string      shape;
    Dimension	dim;

    Screen*     screen;

public:
    Renderer(GameObject* gameObject, const string& shape)
        : Component(gameObject), shape(shape), screen(Screen::GetInstance())
    {}
    virtual ~Renderer() {}

};

