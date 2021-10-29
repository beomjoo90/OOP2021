#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Screen.h"
#include "Input.h"

using namespace std;

class GameObject
{
private:
	char*		shape;
	Position	pos;
	Dimension	dim;
	int			capacity;
	bool		dirty;
	Position	parentPos;

protected:
	Screen*				screen;
	Input*				input;
	vector<GameObject*> children;
	GameObject*			parent;

	void setParentPos(const Position& parentPos) { this->parentPos = parentPos; }

public:

	GameObject(const char* face, const Position& pos, const Dimension& dim, GameObject* parent = nullptr)
		: pos(pos), dim(dim), capacity((size_t)dim.x* dim.y),
		shape{ new char[(size_t)dim.x * dim.y] },
		screen(Screen::GetInstance()), input(Input::GetInstance()), dirty(false), parent(parent)
	{
		setParentPos(parent ? parent->getWorldPos() : Position::zeros);
		if (parent) parent->add(this);

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

	void add(GameObject* child) {
		if (!child) return;
		children.push_back(child);
	}

	Position getPos() const { return pos; } // getter function
	Position getWorldPos() const { return Position{ parentPos.x + pos.x, parentPos.y + pos.y }; }

	Dimension getDimension() const { return dim; }
	void setDimension(const Dimension& dim) { this->dim = dim; }

	void setPos(const Position& pos) {
		this->pos = pos;
		dirty = true;
	}
	void setPos(int x, int y) { setPos(Position{ x, y }); }
	

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

	void internalUpdatePos(bool dirty = false) {
		bool inheritedDirty = dirty;
		if (inheritedDirty == false) {
			if (this->dirty == true) inheritedDirty = true;
		}
		else {
			if (parent) setParentPos(parent->getWorldPos());
		}

		for (auto child : children) child->internalUpdatePos(inheritedDirty);
		this->dirty = false;
	}

	void internalDraw() {
		draw();
		for (auto child : children) child->internalDraw();
	}

	virtual void draw() { screen->draw(getWorldPos(), shape, dim); }
	
	void internalUpdate() {
		update();
		for (auto child : children) child->internalUpdate();
	}

	virtual void update() {}

	void setParent(GameObject* parent) {
		this->parent = parent;
		setParentPos(parent ? parent->getWorldPos() : Position::zeros);
		for (auto child : children) child->internalUpdatePos(true);
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

