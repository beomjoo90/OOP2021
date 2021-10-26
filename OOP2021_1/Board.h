#pragma once
#include "GameObject.h"
class Board :
    public GameObject
{
public:
	Board(const Position& pos, const Dimension& dim)
		: GameObject("", pos, dim)
	{}

	void log(const char* msg) {
		setShape(msg);
	}
};

