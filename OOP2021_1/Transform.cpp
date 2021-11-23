#include "Position.h"
#include "Transform.h"
#include <assert.h>

Transform::Transform(GameObject* gameObject, const Position& pos = Position::zeros,
    const Position& rotation = Position::zeros)
    : Component(gameObject), position(pos), rotation(rotation), dirty(false),
parentWorldPos(Position::zeros)
{
    assert(gameObject != nullptr);
    if (gameObject->parent != nullptr) {
        setParentWorldPos(gameObject->parent->position);
    }
}
