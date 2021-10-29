#pragma once
#include "GameObject.h"
class Block :
    public GameObject
{
	char* internalShapeData; // it is used only in rotateShape().
	bool rotatable;

	void rotateShape() {
		if (rotatable == false) return;

		string shape = getShape();
		Dimension size = getDimension();
		int w = size.x; int h = size.y;
		const char* p = shape.c_str();
		strncpy(internalShapeData, p, w * h);
		for (int y = 0; y < h; y++)
			for (int x = 0; x < w; x++)
				internalShapeData[(w - 1 - x) * h + y] = p[y * w + x];
		setShape(internalShapeData);
		size.x = h; size.y = w;
		setDimension(size);
	}

public:
	Block(const string& shape, const Position& pos, const Position& size, GameObject* parent = nullptr, bool rotatable = true)
		: GameObject(shape.c_str(), pos, size, parent), internalShapeData(new char[size.x * size.y + 1]), rotatable(rotatable) {}

	~Block() {
		if (internalShapeData != nullptr) delete[] internalShapeData;
	}

	void update() override
	{
		if (input->getKey(VK_UP)) rotateShape();
	}

	void draw() override
	{
		Position pos = getWorldPos();
		Dimension size = getDimension();
		screen->drawShape(pos, size, getShape());
	}
};

