#pragma once
#include "GameObject.h"
#include "Behaviour.h"
#include "Transform.h"
#include "MapScript.h"

struct BlockShape {
	string		shape;
	Dimension	dim;
};

class BlockScript :
    public Behaviour
{
	float		currentX;
	float		currentY;
	float		speed;
	GameObject*	map;
	MapScript*	mapScript;
	char*		shapeBuffer;
	

	void rotateShape() {
		auto originalShape = renderer->getShape();
		auto sz = renderer->getDimension();
		int w = sz.x; int h = sz.y;
		for (int y = 0; y < h; y++)
			for (int x = 0; x < w; x++)
				shapeBuffer[(w - 1 - x) * h + y] = originalShape[y * w + x];
		renderer->setShape(shapeBuffer);
		sz.x = h; sz.y = w;
		renderer->setDimension(sz);
	}

public:
	BlockScript(GameObject* gameObject) : Behaviour(gameObject), 
		map( gameObject->getParent() ), shapeBuffer(nullptr), speed(0.1f), currentX(0.0f), currentY(0.0f)
	{
		auto pos = transform->getPos();
		auto dim = renderer->getDimension();
		currentX = (float)pos.x;
		currentY = (float)pos.y;
		shapeBuffer = new char[(size_t)dim.x * dim.y];
		mapScript = map->getComponent<MapScript>();
	}

	~BlockScript() {
		if (shapeBuffer) {
			delete[] shapeBuffer;
		}
	}

	void start() override {
		currentX = 4.0f; currentY = 0.0f;
		speed = 0.1f;
		gameObject->setName("currentBlock");
		transform->setPos((int)currentX, (int)currentY);
	}

	void update() override {
		auto shape = renderer->getShape();
		auto pos = transform->getPos();
		auto dim = renderer->getDimension();
		auto width = dim.x;
		auto height = dim.y;

		if (mapScript->isGrounded(shape, pos, width, height)) {
			mapScript->place(shape, pos, width, height);
			for (int i = height - 1; i >= 0; --i) {
				int nCombos = 0;
				while (mapScript->evaluateLine(pos.y + i)) {
					nCombos++;
				}
			}
			destroy(gameObject);
			return;
		}
		if (input->getKey(VK_RIGHT)) {
			float nextX = currentX + 1.0f;
			if (mapScript->isValidRange({ (int)nextX, (int)currentY }, dim))
				currentX = nextX;
			return;
		}
		if (input->getKey(VK_LEFT)) {
			float nextX = currentX - 1.0f;
			if (mapScript->isValidRange({ (int)nextX, (int)currentY }, dim))
				currentX = nextX;
			return;
		}
		if (input->getKey(VK_UP)) {
			rotateShape();
		}
		if (input->getKey(VK_DOWN)) {
			speed *= 2;
		}
		if (input->getKey(VK_SPACE)) {			
			pos.y = (int)currentY;
			while (!mapScript->isGrounded(shape, pos, width, height)) {
				float nextY = currentY + 1;
				if (mapScript->isValidRange({ (int)currentX, (int)nextY }, dim)) {
					pos.y = (int)nextY;
					currentY = nextY;
				}
			}
			return;
		}
		float nextY = currentY + speed;
		if (mapScript->isValidRange({ (int)currentX, (int)nextY }, dim)) {
			currentY = nextY;
		}
		Borland::Gotoxy(0, 36);
		printf("dim(%d, %d) (%f, %f)\n", dim.x, dim.y, currentX, currentY);

		transform->setPos( (int)currentX, (int)currentY);
	}

	static BlockShape ChooseShape() {
		static vector<BlockShape> candidates{
			{ "\xDB\xDB \xDB \xDB", {2, 3}	},
			{ "\xDB\xDB\xDB\xDB",	{2, 2}	},
			{ "\xDB\xDB\xDB\xDB",	{4, 1}	},
			{ "\xDB\xDB\xDB \xDB ", {2, 3}	},
			{ " \xDB\xDB\xDB\xDB ", {2, 3}	},
			{ " \xDB\xDB\xDB \xDB", {2, 3}	},
			{ "\xDB \xDB\xDB \xDB", {2, 3}  }
		};
		return candidates[rand() % candidates.size()];
	}
};

