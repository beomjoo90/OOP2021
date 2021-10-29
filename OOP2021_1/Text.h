#pragma once
#include "GameObject.h"
class Text :
    public GameObject
{
public:
	Text(const Position& pos, const string& message, GameObject* parent)
		: GameObject(message.c_str(), pos, Dimension{ (int)message.length(), 1 }, parent)
	{}

	void update() override
	{
		Position pos = getPos();
	}
};

