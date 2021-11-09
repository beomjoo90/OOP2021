#pragma once
#include <iostream>
#include <fstream>
#include "Panel.h"
#include "Score.h"

class Map : public Panel {
	bool*		map;
	int			upper;
	Score*		score;

	static const char rectFill = '\xdb';

	void load() {
		ifstream mapFile;
		auto capacity = getCapacity();
		char* data = new char[capacity];
		memset(data, '0', sizeof(char) * capacity);
		mapFile.open("map.txt");
		memset(map, false, sizeof(bool) * capacity);
		if (mapFile.is_open() == false) return;
		
		if (!mapFile.eof()) mapFile.getline(data, capacity);
		
		mapFile.close();
		upper = getHeight() - 1;
		for (int i = capacity-1; i >= 0; i--)
		{
			if (data[i] == '0') continue;
			map[i] = true;
			auto pos = offset2Pos(i);
			if (pos.y < upper) upper = pos.y;
		}
		delete[] data;
	}

public:
	Map(const Position& pos, int width, int height, GameObject* parent)
		: map(new bool[(size_t)width * height]), upper(height - 1),
		Panel("", pos, width, height, parent), score(nullptr)
	{
		for (int i = 0; i < width * height; i++) 
			map[i] = false;
		load();
	}

	~Map() { if (map) delete[] map; }

	void setScore(Score* score) {
		this->score = score;
	}

	bool isDone() const { return upper <= 0; }

	void addScore(int inc) {
		if (!score) return;
		score->setValue(score->getValue() + inc);
	}

	bool isLineAllOccupied(int line) const {
		for (int i = 0; i < dim.x; i++) 
			if (!map[line * dim.x + i]) 
				return false;
		return true;
	}

	bool evaluateLine(int line) {
		if (!isLineAllOccupied(line)) return false;
		addScore(1);
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
		auto pos = getPos();
		if (nextPos.x < 0 || nextPos.x > getWidth() || nextPos.y < 0 || nextPos.y > getHeight() ) return false;
		if (nextPos.x + sz.x > getWidth() || nextPos.y + sz.y > getHeight()) return false;
		return true;
	}

	bool isGrounded(const char* shape, const Position& pos, int w, int h) {
		auto height = dim.y;
		auto width = dim.x;
		if (pos.y + h >= height) return true;

		auto next = pos.y + h;		
		if (next < upper) return false;
		auto capacity = getCapacity();
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
		return map[pos2Offset(pos)];
	}

	void place(const char* shape, const Position& pos, int w, int h) {
		auto worldPos = local2Screen();
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				if (shape[j + i * w] != ' ') {
					map[pos.x + j + (pos.y + i) * dim.x] = true;
					setShape(rectFill, { pos.x + j, pos.y + i });
				}
			}
		}
		if (pos.y < upper) upper = pos.y;
	}

	void draw()
	{
		Panel::draw(); // draw border lines

		auto height = dim.y;
		auto width = dim.x;
		for (auto i = max(upper-4,0); i < height; i++) {
			for (auto j = 0; j < width; j++) {
				setShape(map[j + i * width] ? rectFill : ' ', j + i * width);
			}
		}
		GameObject::draw(); // draw shape
	}
};