#include "Position.h"

Position Position::up{ 0, -1 };
Position Position::down{ 0, 1 };
Position Position::right{ 1, 0 };
Position Position::left{ -1, 0 };
Position Position::zeros{ 0, 0 };
Position Position::ones{ 1, 1 };

ostream& operator<<(ostream& o, const Position& pos) { 
	o << "(" << pos.x << "," << pos.y << ")"; return o;
}

// 

Position operator*(int x, Position pos) { return pos.operator*(x); }

Position operator*(int x, const Position& pos) { return pos.operator*(x); }
