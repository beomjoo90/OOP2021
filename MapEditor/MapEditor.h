#pragma once
#include <iostream>
#include <fstream>
#include "Panel.h"

class MapEditor : public Panel {
	bool*		map;

	static const char rectFill = '\xdb';

	bool isInside(const Position& pos) const {
		auto myPos = getPos();
		return 0 <= pos.x && dim.x >= pos.x && 0 <= pos.y && dim.y >= pos.y;
	}

public:
	MapEditor(const Position& pos, int width, int height, GameObject* parent)
		: map(new bool[(size_t)width * height]),
		Panel("", pos, width, height, parent)
	{
		for (int i = 0; i < width * height; i++) 
			map[i] = false;
		load();
	}

	~MapEditor() { if (map) delete[] map; }

	bool isValidRange(const Position& nextPos, const Dimension& sz)
	{
		auto pos = getPos();
		if (nextPos.x < 0 || nextPos.x > getWidth() || nextPos.y < 0 || nextPos.y > getHeight() ) return false;
		if (nextPos.x + sz.x > getWidth() || nextPos.y + sz.y > getHeight()) return false;
		return true;
	}

	void save() {
		ofstream mapFile;

		mapFile.open("map.txt", ofstream::trunc);
		auto capacity = getCapacity();
		for (int i = 0; i < capacity; i++)
		{
			mapFile << map[i] ? '1' : '0';
		}
		mapFile.close();
	}

	void load() {
		ifstream mapFile;
		auto capacity = getCapacity();
		char* data = new char[capacity];
		mapFile.open("map.txt");
		memset(map, false, sizeof(bool) * capacity);
		if (mapFile.is_open() == false) {
			return;
		}
		if (!mapFile.eof()) {
			mapFile.getline(data, capacity);
		}
		mapFile.close();
		for (int i = 0; i < capacity; i++)
		{
			if (data[i] == '1') map[i] = true;
		}
		delete[] data;
	}

	void update()
	{
		GameObject::update();
		if (input->getMouseButtonDown(0)) {
			auto pos = screen2Local(input->getMousePosition());
			if (isInside(pos)) {
				auto offset = pos2Offset(pos);
				map[offset] = !map[offset];
				setShape(map[offset] ? rectFill : ' ', offset);
			}
		}
	}

	void draw()
	{
		Panel::draw(); // draw border lines

		auto height = dim.y;
		auto width = dim.x;
		for (auto i = 0; i < height; i++) {
			for (auto j = 0; j < width; j++) {
				setShape(map[j + i * width] ? rectFill : ' ', j + i * width);
			}
		}
		GameObject::draw(); // draw shape
	}
};