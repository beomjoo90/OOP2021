#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <conio.h> // console io
#include <cstring> // string.h
#include <cstdlib> // stdlib.h
#include <string> // c++ string class
#include <Windows.h>
#include "Utils.h"0

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

static HANDLE hStdin;
static DWORD fdwSaveOldMode;

static char blankChars[80];

static void ErrorExit(const char*);
static void KeyEventProc(KEY_EVENT_RECORD);
static void MouseEventProc(MOUSE_EVENT_RECORD);
static void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);


int main()
{	
	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128];
	int counter = 0;
	
	
	memset(blankChars, ' ', 80);
	blankChars[79] = '\0';

	// Get the standard inp

	Screen  screen(20, 10);
	Position pos{ 1, 2 };
	char shape[] = "**    **     **";
	Dimension sz{ (int)strlen(shape), 1 };


	// Get the standard input handle.

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");
	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		ErrorExit("GetConsoleMode");
	/*
		   Step-1:
		   Disable 'Quick Edit Mode' option programmatically
	 */
	fdwMode = ENABLE_EXTENDED_FLAGS;
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit("SetConsoleMode");
	/*
	   Step-2:
	   Enable the window and mouse input events,
	   after you have already applied that 'ENABLE_EXTENDED_FLAGS'
	   to disable 'Quick Edit Mode'
	*/
	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit("SetConsoleMode");

	bool isLooping = true;
	while (isLooping) {
	
		screen.clear();

		if (GetNumberOfConsoleInputEvents(hStdin, &cNumRead)) {
			Borland::gotoxy(0, 14);
			printf("number of inputs %d\n", cNumRead);

			if (cNumRead > 0) {

				if (!ReadConsoleInput(
					hStdin,      // input buffer handle
					irInBuf,     // buffer to read into
					128,         // size of read buffer
					&cNumRead)) // number of records read
					ErrorExit("ReadConsoleInput");
				// Dispatch the events to the appropriate handler.

				for (i = 0; i < cNumRead; i++)
				{
					switch (irInBuf[i].EventType)
					{
					case KEY_EVENT: // keyboard input
						KeyEventProc(irInBuf[i].Event.KeyEvent);
						break;

					case MOUSE_EVENT: // mouse input
						MouseEventProc(irInBuf[i].Event.MouseEvent);
						break;

					case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
						ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
						break;

					case FOCUS_EVENT:  // disregard focus events

					case MENU_EVENT:   // disregard menu events
						break;

					default:
						ErrorExit("Unknown event type");
						break;
					}
				}
			}
			Borland::gotoxy(0, 0);
		}

		
		screen.draw(pos, shape, sz);

		screen.render();
		Sleep(100);

		pos.x = (pos.x + 1) % (screen.getWidth());
		
	}
	printf("\nGame Over\n");

	SetConsoleMode(hStdin, fdwSaveOldMode);

	return 0;
}

void ErrorExit(const char* lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);
	
	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

void KeyEventProc(KEY_EVENT_RECORD ker)
{
	Borland::gotoxy(0, 11);
	printf("%s\r", blankChars);
	switch (ker.wVirtualKeyCode) {
	case VK_LBUTTON:
		printf("left button ");
		break;
	case VK_BACK:
		printf("back space");
		break;
	case VK_RETURN:
		printf("enter key");
		break;
	case VK_LEFT:
		printf("arrow left");
		break;
	case VK_UP:
		printf("arrow up");
		break;
	default:
		if (ker.wVirtualKeyCode >= 0x30 && ker.wVirtualKeyCode <= 0x39)
			printf("Key event: %c ", ker.wVirtualKeyCode - 0x30 + '0');
		else printf("Key event: %c ", ker.wVirtualKeyCode - 0x41 + 'A');
		break;
	}
	
	Borland::gotoxy(0, 0);
}

void MouseEventProc(MOUSE_EVENT_RECORD mer)
{
	Borland::gotoxy(0, 12);
	printf("%s\r", blankChars);
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	printf("Mouse event: ");

	switch (mer.dwEventFlags)
	{
	case 0:
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			printf("left button press %d %d\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			printf("right button press \n");
		}
		else
		{
			printf("button press\n");
		}
		break;
	case DOUBLE_CLICK:
		printf("double click\n");
		break;
	case MOUSE_HWHEELED:
		printf("horizontal mouse wheel\n");
		break;
	case MOUSE_MOVED:
		printf("mouse moved %d %d\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
		break;
	case MOUSE_WHEELED:
		printf("vertical mouse wheel\n");
		break;
	default:
		printf("unknown\n");
		break;
	}
	Borland::gotoxy(0, 0);
}

void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	Borland::gotoxy(0, 13);
	printf("%s\r", blankChars);
	printf("Resize event: ");
	printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
	Borland::gotoxy(0, 0);
}