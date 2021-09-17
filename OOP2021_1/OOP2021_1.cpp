#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <conio.h> // console io
#include <cstring> // string.h
#include <cstdlib> // stdlib.h
#include <string> // c++ string class
#include <Windows.h>
#include "Utils.h"

// https://github.com/beomjoo90/OOP2021 , branch: 2학기

class Screen;
class GameObject;

class Screen {
private:
	int		width; // visible width
	int		height;
	int		size;
	char*	canvas;

public:

	// constructor (생성자 함수) 메모리공간상에 적재되는 순간 호출되는
	Screen(int width = 10, int height = 10) 
		: width(width), height(height), canvas( new char[(width+1)*height])
	{
		bool faultyInput = false;
		if (this->width <= 0) {
			this->width = 10;
			faultyInput = true;
		}
		if (this->height <= 0) {
			this->height = 10;
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
		delete[] canvas;
		canvas = nullptr;
		width = 0; height = 0;
	}

	int getWidth()
	{
		return width;
	}

	void clear()
	{
		memset(canvas, ' ', size);
	}
	void draw(const Position& pos, const char* shape, const Dimension& sz = Position{ 1, 1 } )
	{
		int offset = (width + 1) * pos.y + pos.x;
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
class GameObject
{
private:
	char	face[20];
	int		pos;
	int		direction;
	Screen* screen;
	GameObject** gameObjects;

public:

	GameObject(GameObject** gameObjects, Screen* screen, const char* face, int pos, int direction) 
		: pos(pos), direction(direction), screen(screen), gameObjects(gameObjects)
	{
		setFace(face);
	}
	virtual ~GameObject() {}

	void move(int direction)
	{	
	}
	void move()
	{	
	}
	virtual void draw()
	{	
	}
	virtual void update() {}

	int getPos() { return pos; } // getter function
	void setPos(int pos) { this->pos = pos; } // setter function

	int getDirection() { return direction;  }
	void setDirection(int direction) { this->direction = direction; }

	const char* getFace() { return face;  }
	void setFace(const char* face) { strcpy(this->face, face); }

	Screen* getScreen() { return screen; }
	GameObject** getGameObjects() { return gameObjects; }
};


int main()
{	
	int major;
	int minor;

	Screen  screen(20, 10);
	Position pos{ 1, 2 };
	char shape[] = "**    **     **";
	Dimension sz{ (int)strlen(shape), 1 };

	bool isLooping = true;
	while (isLooping) {
		screen.clear();

		screen.draw(pos, shape, sz);

		screen.render();
		Sleep(100);

		pos.x = (pos.x + 1) % (screen.getWidth());
		
	}
	printf("\nGame Over\n");

	return 0;
}