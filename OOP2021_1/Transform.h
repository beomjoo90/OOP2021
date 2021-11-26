#pragma once
#include "Component.h"
#include "Position.h"
#include <assert.h>

class Transform :
    public Component
{
    Position    localPosition;
    Position    rotation;

    bool        dirty;          // if its pos is updated, it should be marked as dirty
								// to make sure all the locations of its children game objects
								// are updated accordingly.
    Position	parentWorldPos; // "absolute" position of its parent game object to the screen 

public:
    Transform(GameObject* gameObject, const Position& pos = Position::zeros,
        const Position& rotation = Position::zeros);

    void setParentWorldPos(const Position& parentWorldPos) { this->parentWorldPos = parentWorldPos; }

    Position getPosition() const { return localPosition; }

    Position local2World(const Position& pos) const { return parentWorldPos + pos; }
    Position local2World() const { return local2World(this->localPosition); }
    
    Position world2Local(const Position& screenPos) const {
        return screenPos - local2World();
    }

    void move(const Position& offset) {}

    Position getPos() const { return localPosition; }
    void setPos(const Position& pos) { this->localPosition = pos; dirty = true; }
    void setPos(int x, int y) { setPos(Position{ x, y }); }

};

