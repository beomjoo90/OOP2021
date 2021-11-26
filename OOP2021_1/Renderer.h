#pragma once
#include "Screen.h"
#include "Component.h"

class Renderer :
    public Component
{
    string      shape;
    Dimension	dim;
    int         capacity;
    Transform*  transform;
    Screen*     screen;

public:
    Renderer(GameObject* gameObject, const char* face, const Dimension& dim)
        :   Component(gameObject), transform( getTransform()), shape(face), dim(dim), capacity((size_t)dim.x* dim.y),
            screen(Screen::GetInstance())
    {
        if (face == nullptr || shape == "") {
            shape.assign(capacity, ' ');
        }
    }
    virtual ~Renderer() {}

    int getWidth() const { return dim.x; }
    int getHeight() const { return dim.y; }

    int getCapacity() const { return capacity; }
};

