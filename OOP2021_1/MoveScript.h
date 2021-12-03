#pragma once
#include "Behaviour.h"
#include "Transform.h"

class MoveScript :
    public Behaviour
{
public:
    MoveScript(GameObject* obj) : Behaviour(obj) {
        
    }

    void update() override {
        auto pos = transform->getPos();
        if (input->getKeyDown('W')) {
            transform->setPos(pos + Position::up);
        }
        if (input->getKeyDown('D')) {
            transform->setPos(pos + Position::right);
        }
        if (input->getKeyDown('A')) {
            transform->setPos(pos + Position::left);
        }
        if (input->getKeyDown('S')) {
            transform->setPos(pos + Position::down);
        }
    }
};

