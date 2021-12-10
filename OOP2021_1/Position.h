#pragma once
#include <ostream>
using namespace std;

struct Position
{
	int x;
	int y;

	Position(int x = 0, int y = 0) : x(x), y(y) {}
	Position(const Position& other) : Position(other.x, other.y) {} // copy constructor

	Position& operator+=(const Position& other) {
		this->x += other.x; this->y += other.y;
		return *this; // chaining
	}
	Position operator+(const Position& other) const { 
		return Position{ this->x + other.x, this->y + other.y }; 
	}
	Position operator-(const Position& other) const { 
		return Position{ this->x - other.x, this->y - other.y }; 
	}
	Position operator*(int x) const {  // pos.operator*(2) X 2*pos
		return Position{ this->x * x, this->y * x }; 
	}
	Position& operator=(const Position& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	bool operator==(const Position& other) const { return this->x == other.x && this->y == other.y; }

	bool operator!=(const Position& other) const { return !(*this == other); }

	friend ostream& operator<<(ostream& o, const Position& pos);
	friend Position operator*(int x, Position pos);
	friend Position operator*(int x, const Position& pos);

	static Position up;
	static Position right;
	static Position down;
	static Position left;
	static Position zeros;
	static Position ones;

};

typedef Position Dimension;

