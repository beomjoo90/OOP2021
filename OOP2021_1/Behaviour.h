#pragma once
#include "Input.h"
#include "Component.h"

class Behaviour :
    public Component
{
protected:
    Input*      input;

public:
    Behaviour(GameObject* gameObject) : Component(gameObject), input(Input::GetInstance()) {}
    virtual ~Behaviour() {}
};

