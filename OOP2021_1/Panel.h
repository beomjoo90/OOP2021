#pragma once
#include "GameObject.h"
class Panel :
    public GameObject
{
	int width;
	int height;
	string title;

public:
	Panel(const string& title, const Position& pos, int width, int height, GameObject* parent)
		: GameObject("", pos, Dimension{ width, height }, parent),
		width(width), height(height), title(title)
	{}

	void update() override
	{
		Position pos = getPos();
	}

	void draw() override
	{
		Position pos = getWorldPos();
		screen->drawRectangle(Position{ pos.x - 1, pos.y - 1 }, Position{ width + 2, height + 2 });
		if (title.length() > 0) screen->draw(Position{ pos.x + 1, pos.y - 1 }, title.c_str());
	}
};

