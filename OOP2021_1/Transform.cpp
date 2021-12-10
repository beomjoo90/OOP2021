#include "Position.h"
#include "GameObject.h"
#include "Transform.h"
#include <assert.h>

Transform::Transform(GameObject* gameObject, const Position& pos, const Position& rotation)
    : Component(gameObject), localPosition(pos), rotation(rotation), dirty(true)
{
    assert(gameObject != nullptr);
}
