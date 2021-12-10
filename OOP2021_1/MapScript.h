#pragma once
#include "GameObject.h"
#include "Behaviour.h"
#include "ScoreScript.h"
class MapScript :
    public Behaviour
{
	bool*			map;
	int				upper;
	ScoreScript*	score;

	static const char rectFill = '\xdb';

public:
	MapScript(GameObject* gameObject) : Behaviour(gameObject), 
		map(nullptr), upper(0), score(nullptr)
	{
		auto dim = renderer->getDimension();
		map = new bool[(size_t)dim.x * dim.y];
		upper = dim.y - 1;
		for (int i = 0; i < dim.x * dim.y; i++) map[i] = false;
	}

	bool isDone() const { return upper <= 0; }
	void setScore(ScoreScript* score) { this->score = score; }

	void addScore(int inc) {
		if (!score) return;
		score->setValue(score->getValue() + inc);
	}

	bool isLineAllOccupied(int line) const {
		auto dim = renderer->getDimension();
		for (int i = 0; i < dim.x; i++)
			if (!map[line * dim.x + i])
				return false;
		return true;
	}

	bool evaluateLine(int line) {
		if (!isLineAllOccupied(line)) return false;
		addScore(1);
		auto dim = renderer->getDimension();
		auto width = dim.x;

		// clean the given line
		memset(&map[line * width], false, sizeof(bool) * width);

		// copy lines above the "line" down below their below lines.
		for (auto i = line - 1; i >= (upper - 1) && i >= 0; i--) {
			memcpy(&map[(i + 1) * width], &map[i * width], sizeof(bool) * width);
		}

		upper++;
		return true;
	}

	bool isValidRange(const Position& nextPos, const Dimension& sz)
	{
		auto pos = transform->getPos();
		if (nextPos.x < 0 || nextPos.x > renderer->getWidth() || nextPos.y < 0 || nextPos.y > renderer->getHeight()) return false;
		if (nextPos.x + sz.x > renderer->getWidth() || nextPos.y + sz.y > renderer->getHeight()) return false;
		return true;
	}

	bool isGrounded(const char* shape, const Position& pos, int w, int h) {
		auto dim = renderer->getDimension();
		auto height = dim.y;
		auto width = dim.x;
		if (pos.y + h >= height) return true;

		auto next = pos.y + h;
		if (next < upper) return false;
		auto capacity = renderer->getCapacity();
		for (auto j = 0; j < w; j++) {
			auto i = 0;
			auto last = -1;
			for (i = 0; i < h; i++) {
				if (shape[j + i * w] != ' ') last = i;
			}
			if (last == -1) continue;
			auto nextOffset = pos.x + j + (pos.y + last + 1) * width;
			if (nextOffset >= capacity) return true;
			if (map[nextOffset] == true) return true;
		}
		return false;
	}

	bool isOccupied(const Position& pos) {
		return map[renderer->pos2Offset(pos)];
	}

	void place(const char* shape, const Position& pos, int w, int h) {
		auto dim = renderer->getDimension();
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				if (shape[j + i * w] != ' ') {
					map[pos.x + j + (pos.y + i) * dim.x] = true;
					renderer->setShape(rectFill, { pos.x + j, pos.y + i });
				}
			}
		}
		if (pos.y < upper) upper = pos.y;
	}

	void update()
	{
		auto dim = renderer->getDimension();
		auto height = dim.y;
		auto width = dim.x;
		for (auto i = max(upper - 4, 0); i < height; i++) {
			for (auto j = 0; j < width; j++) {
				renderer->setShape(map[j + i * width] ? rectFill : ' ', j + i * width);
			}
		}
	}
};

