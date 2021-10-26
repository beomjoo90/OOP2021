#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include "Screen.h"
#include "Input.h"

using namespace std;

class GameObject
{
private:
	char* shape;
	Position	pos;
	Dimension	dim;
	int			capacity;

protected:
	Screen* screen;
	Input* input;
	vector<GameObject*> children;

public:

	GameObject(const char* face, const Position& pos, const Dimension& dim)
		: pos(pos), dim(dim), capacity((size_t)dim.x* dim.y),
		shape{ new char[(size_t)dim.x * dim.y] },
		screen(Screen::GetInstance()), input(Input::GetInstance())
	{
		if (face == nullptr || face[0] == '\0') {
			memset(shape, ' ', sizeof(char) * capacity);
			return;
		}
		strncpy(shape, face, min(strlen(face), capacity));
	}

	virtual ~GameObject()
	{
		while (children.size() > 0)
		{
			auto child = children.back();
			children.pop_back();
			delete child;
		}
		if (shape != nullptr)
			delete[] shape;
		shape = nullptr;
	}

	virtual void move(int pos) {}
	virtual void move(const Position& offset) {}
	virtual void move() {}

	virtual void draw() {
		screen->draw(pos, shape, dim);
		for (auto child : children) child->draw();
	}

	virtual void update() {
		for (auto child : children) child->update();
	}

	Position getPos() const { return pos; } // getter function
	void setPos(const Position& pos) { this->pos = pos; } // setter function

	const char* getShape() const { return shape; }
	void setShape(const char* face) {
		if (face == nullptr || face[0] == '\0') {
			memset(shape, ' ', sizeof(char) * capacity);
			return;
		}
		strncpy(shape, face, max(capacity, strlen(face)));
	}
	void setShape(char shape, int pos)
	{
		if (pos < 0 || pos >= capacity) return;
		this->shape[pos] = shape;
	}

	Position screen2local(const Position& screenPosition) const {
		Position pos = getPos();
		return Position{ screenPosition.x - pos.x, screenPosition.y - pos.y };
	}
	Position local2screen(const Position& target) const {
		Position pos = getPos();
		return { pos.x + target.x, pos.y + target.y };
	}
};

