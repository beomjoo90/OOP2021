#include "Position.h"
#include "GameObject.h"
#include "Transform.h"
#include <assert.h>

Transform::Transform(GameObject* gameObject, const Position& pos = Position::zeros,
    const Position& rotation = Position::zeros)
    : Component(gameObject), localPosition(pos), rotation(rotation), dirty(false),
      parentWorldPos(Position::zeros)
{
    assert(gameObject != nullptr);
    auto parent = getParent();
    if (parent) setParentWorldPos( parent->getTransform()->local2World() );
}
