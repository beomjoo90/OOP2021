#pragma once
struct Position
{
	int x;
	int y;

	Position(int x = 0, int y = 0) : x(x), y(y) {}
	Position(const Position& other) : Position(other.x, other.y) {} // copy constructor


	static Position up;
	static Position right;
	static Position down;
	static Position left;
	static Position zeros;
	static Position ones;

};

typedef Position Dimension;

