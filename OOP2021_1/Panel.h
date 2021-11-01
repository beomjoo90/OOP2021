#pragma once
#include "GameObject.h"
class Panel :
    public GameObject
{
	string title;

public:
	Panel(const string& title, const Position& pos, int width, int height, GameObject* parent)
		: GameObject("", pos, Dimension{ width, height }, parent), title(title)
	{}

	void draw() override
	{
		Position pos = local2Screen();
		screen->drawRectangle( { pos.x - 1, pos.y - 1 }, { dim.x + 1, dim.y + 1 });
		if (title.length() == 0) return;
		screen->draw( { pos.x + 1, pos.y - 1 }, title.c_str());
	}
};

