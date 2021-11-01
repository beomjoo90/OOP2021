#pragma once
#include "GameObject.h"

class Score :
    public GameObject
{
	int value;
	char buffer[3];

public:
	Score(const Position& pos, GameObject* parent)
		: GameObject("0", pos, Dimension{ 3, 1 }, parent), value(0)
	{
		sprintf(buffer, "%3d", value % 1000);
	}

	void draw() override
	{
		sprintf(buffer, "%3d", value % 1000);
		screen->draw(local2Screen(), buffer);
	}

	void setValue(int value) { this->value = value; }

	int getValue() const { return value;  }
};

