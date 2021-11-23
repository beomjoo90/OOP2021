#pragma once
#include "Component.h"
#include "Position.h"
#include <assert.h>

class Transform :
    public Component
{
    Position    position;
    Position    rotation;

    bool        dirty;          // if its pos is updated, it should be marked as dirty
								// to make sure all the locations of its children game objects
								// are updated accordingly.
    Position	parentWorldPos; // "absolute" position of its parent game object to the screen 

public:
    Transform(GameObject* gameObject, const Position& pos = Position::zeros,
        const Position& rotation = Position::zeros)
        : Component(gameObject), position(pos), rotation(rotation), dirty(false),
          parentWorldPos(Position::zeros)
    {
        assert(gameObject != nullptr);
        if (gameObject->parent != nullptr) {
            setParentWorldPos(gameObject->parent->position);
        }
    }


    void setParentWorldPos(const Position& parentWorldPos) 
    { this->parentWorldPos = parentWorldPos; }

};

