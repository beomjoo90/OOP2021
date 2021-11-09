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

	bool validateRange(int offset) const { return offset >= 0 || offset < size; }

public:

	static Screen* GetInstance() {
		if (Instance == nullptr) {
			Instance = new Screen(80, 35);
		}
		return Instance;
	}

	int getWidth() const { return width; }
	void setWidth(int width) { this->width = width; }

	int getHeight() const { return height; }

	int pos2Offset(const Position& pos) const { 
		if (pos.x < 0 || pos.x > width || pos.y < 0 || pos.y > height) return -1;
		return (width + 1) * pos.y + pos.x; 
	}

	void clear() { memset(canvas, ' ', size); }
	
	void draw(const Position& pos, const char* shape, const Dimension& dim) {
		int offset = pos2Offset(pos);
		if (validateRange(offset) < 0) return;
		for (int h = 0; h < dim.y; h++)
			strncpy(&canvas[offset + (width + 1) * h], &shape[h * dim.x], dim.x);
	}
	void draw(int x, int y, char shape) {
		int offset = pos2Offset({ x, y });
		if (validateRange(offset) < 0) return;
		canvas[offset] = shape;
	}
	void draw(const Position& pos, char shape) { draw(pos.x, pos.y, shape); }
	void draw(int x, int y, const char* shape, int len) { draw(Position{ x, y }, shape, Dimension{ len, 1 }); }
	void draw(const Position& pos, const char* shape) { draw(pos.x, pos.y, shape, strlen(shape)); }
	void draw(const Position& pos, const char* shape, int len) { draw(pos.x, pos.y, shape, len); }
	void drawLineHorizontal(const Position& pos, int width) {
		if (pos.x < 0 || pos.y < 0 || pos.x + width > getWidth() || pos.y > getHeight()) return;
		for (int i = pos.x; i <= min(this->width, pos.x + width); i++) 
			draw(i, pos.y, 205);
	}
	void drawLineVertical(const Position& pos, int height) {
		if (pos.x < 0 || pos.y < 0 || pos.x > getWidth() || pos.y + height > getHeight()) return;
		for (int i = pos.y; i <= min(this->height, pos.y + height); i++) 
			draw(pos.x, i, 186);
	}
	void drawRectangle(const Position& topLeft, const Dimension& sz) {
		drawLineHorizontal(topLeft, sz.x);
		drawLineHorizontal(Position{ topLeft.x, topLeft.y + sz.y }, sz.x);
		drawLineVertical(topLeft, sz.y);
		drawLineVertical(Position{topLeft.x + sz.x, topLeft.y }, sz.y);
		draw(topLeft, 201); 
		draw(Position{ topLeft.x + sz.x, topLeft.y }, 187);
		draw(topLeft.x, topLeft.y + sz.y, 200); 
		draw(topLeft.x + sz.x, topLeft.y + sz.y, 188);
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
