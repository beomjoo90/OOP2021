#pragma once
#include "Component.h"
#include "Position.h"
#include "Input.h"
#include <assert.h>

class Transform :
    public Component
{
    Position    localPosition;
    Position    rotation;

    bool        dirty;          // if its pos is updated, it should be marked as dirty
								// to make sure all the locations of its children game objects
								// are updated accordingly.

public:
    Transform(GameObject* gameObject, 
        const Position& pos = Position::zeros,
        const Position& rotation = Position::zeros);

    Position getPosition() const { return localPosition; }

    Position local2World(const Position& parentWorldPos, const Position& pos) const { return parentWorldPos + pos; }
    Position local2World(const Position& parentWorldPos) const { return parentWorldPos + this->localPosition; }

    void move(const Position& offset) {}

    Position getPos() const { return localPosition; }
    void setPos(const Position& pos) { dirty = pos != this->localPosition;  this->localPosition = pos; }
    void setPos(int x, int y) { setPos(Position{ x, y }); }

	void setDirty(bool dirty) { this->dirty = dirty; }
	bool getDirty() const { return dirty; }
};

