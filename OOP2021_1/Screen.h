#pragma once

#include <iostream>
#include "Position.h"
#include "Borland.h"

class Screen
{
private:
	int		width; // visible width
	int		height;
	int		size;
	char* canvas;

	// constructor (생성자 함수) 메모리공간상에 적재되는 순간 호출되는
	Screen(int width = 80, int height = 20)
		: width(width), height(height),
		canvas(new char[((size_t)width + 1) * height])
	{
		bool faultyInput = false;
		if (this->width <= 0) {
			this->width = 80;
			faultyInput = true;
		}
		if (this->height <= 0) {
			this->height = 20;
			faultyInput = true;
		}
		size = (this->width + 1) * this->height;
		if (faultyInput == true) {
			delete canvas;
			canvas = new char[size];
		}
	}
	// destructor (소멸자 함수) 메모리공간상에서 없어지는 순간 호출되는 함수
	virtual ~Screen()
	{
		if (canvas != nullptr)
			delete[] canvas;
		canvas = nullptr;
		width = 0; height = 0;
	}

	static Screen* Instance;

public:

	static Screen* GetInstance() {
		if (Instance == nullptr) {
			Instance = new Screen(80, 40);
		}
		return Instance;
	}

	int getWidth() const { return width; }

	void setWidth(int width) { this->width = width; }

	void clear() { memset(canvas, ' ', size); }

	int pos2Offset(const Position& pos) const { return (width + 1) * pos.y + pos.x; }

	void draw(const Position& pos, const char* shape, const Dimension& sz = Position{ 1, 1 })
	{
		int offset = pos2Offset(pos);
		for (int h = 0; h < sz.y; h++)
			strncpy(&canvas[offset + (width + 1) * h], &shape[h * sz.x], sz.x);
	}

	void render()
	{
		Borland::gotoxy(0, 0);
		for (int h = 0; h < height; h++)
			canvas[(width + 1) * (h + 1) - 1] = '\n';
		canvas[size - 1] = '\0';
		printf("%s", canvas);
	}
};
