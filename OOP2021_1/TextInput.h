#pragma once
#include "GameObject.h"
class TextInput :
    public GameObject
{
	int value;

public:
	TextInput(int data, const Position& pos, GameObject* parent)
		: GameObject("", pos, Dimension{ 4, 1 }, parent), value(data)
	{}

	void update() override
	{	
	}

	void draw() override
	{
		static char buffer[10];
		sprintf(buffer, "%3d\0", value);
		Position pos = getWorldPos();
		screen->draw(pos, buffer);
	}

	void setValue(int value) { this->value = value; }
};

