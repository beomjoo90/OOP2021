#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <conio.h> // console io
#include <cstring> // string.h
#include <cstdlib> // stdlib.h
#include <string>
#include <string> // c++ string class
#include <Windows.h>
#include "Utils.h"

// https://github.com/beomjoo90/OOP2021 , branch: 2학기

class Screen;
class GameObject;
class Input;

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

	int getWidth() const
	{
		return width;
	}

	void setWidth(int width)
	{
		this->width = width;
	}

	void clear() 
	{
		memset(canvas, ' ', size);
	}
	Position offset2Pos(int offset) const 
	{

	}

	int pos2Offset(const Position& pos) const 
	{
		return (width + 1) * pos.y + pos.x;
	}

	void draw(const Position& pos, const char* shape, const Dimension& sz = Position{ 1, 1 } ) 
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

class Input {
	static DWORD cNumRead, fdwMode, i;
	static INPUT_RECORD irInBuf[128];
	static int counter;
	static HANDLE hStdin;
	static DWORD fdwSaveOldMode;
	static char blankChars[80];

	static void ErrorExit(const char*);
	static void KeyEventProc(KEY_EVENT_RECORD);
	static void MouseEventProc(MOUSE_EVENT_RECORD);
	static void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

public:
	static void Initialize()
	{
		memset(blankChars, ' ', 80);
		blankChars[79] = '\0';

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

	}
	static void Deinitialize() {
		SetConsoleMode(hStdin, fdwSaveOldMode);
	}
	static void ReadInputs() 
	{
		if (!GetNumberOfConsoleInputEvents(hStdin, &cNumRead)) {
			cNumRead = 0;
			return;
		}
		if (cNumRead == 0) return;

		Borland::gotoxy(0, 14);
		printf("number of inputs %d\n", cNumRead);

		if (!ReadConsoleInput(
			hStdin,      // input buffer handle
			irInBuf,     // buffer to read into
			128,         // size of read buffer
			&cNumRead)) // number of records read
			ErrorExit("ReadConsoleInput");
		// Dispatch the events to the appropriate handler.

#ifdef NOT_COMPILE
			for (int i = 0; i < cNumRead; i++)
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
#endif 

		Borland::gotoxy(0, 0);
	}
	static bool GetKeyDown(WORD virtualKeyCode);
	static bool GetKey(WORD virtualKeyCode);
	static bool GetKeyUp(WORD virtualKeyCode);
};

DWORD Input::cNumRead, Input::fdwMode;
INPUT_RECORD Input::irInBuf[128];
int Input::counter;
HANDLE Input::hStdin;
DWORD Input::fdwSaveOldMode;
char Input::blankChars[80];


class GameObject
{
private:
	char		face[20];
	Position	pos;
	Dimension	dim;
	Screen*		screen;

public:

	GameObject(Screen* screen, const char* face, const Position& pos, const Dimension& dim)
		: pos(pos), screen(screen), dim(dim)
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
		screen->draw(pos, face, dim);
	}

	virtual void update() 
	{	
		if (Input::GetKeyUp(VK_LEFT)) {
			if (pos.x <= 0) return;
			pos.x = (pos.x - 1) % (screen->getWidth());
		}
		if (Input::GetKeyUp(VK_RIGHT)) {
			if (pos.x >= (screen->getWidth() - 1)) return;
			pos.x = (pos.x + 1) % (screen->getWidth());
		}
	}

	Position getPos() { return pos; } // getter function
	void setPos(const Position& pos) { this->pos = pos; } // setter function

	const char* getFace() { return face; }
	void setFace(const char* face) { strcpy(this->face, face); }

	Screen* getScreen() { return screen; }
};

int main()
{	
	Screen  screen(20, 10);
	Position pos{ 1, 2 };
	const char shape[] = "**    **     **";
	Dimension sz{ (int)strlen(shape), 1 };
	GameObject one{ &screen, shape, pos,  sz };

	// Get the standard inp
	Input::Initialize();	   	

	bool isLooping = true;
	while (isLooping) {
	
		screen.clear();
		one.draw();

		Input::ReadInputs();

		one.update();

		screen.render();
		Sleep(100);
		
	}
	printf("\nGame Over\n");

	Input::Deinitialize();

	return 0;
}

void Input::ErrorExit(const char* lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);
	
	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

bool Input::GetKeyDown(WORD virtualKeyCode)
{
	if (cNumRead == 0) return false;

	for (int i = 0; i < cNumRead; i++)
	{
		if (irInBuf[i].EventType != KEY_EVENT) continue;

		if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == virtualKeyCode &&
			irInBuf[i].Event.KeyEvent.bKeyDown == TRUE) {
			return true;
		}
	}
	return false;
}

bool Input::GetKey(WORD virtualKeyCode)
{
	if (cNumRead == 0) return false;

	for (int i = 0; i < cNumRead; i++)
	{
		if (irInBuf[i].EventType != KEY_EVENT) continue;

		if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == virtualKeyCode &&
			irInBuf[i].Event.KeyEvent.bKeyDown == TRUE ) {
			return true;
		}
	}
	return false;
}

bool Input::GetKeyUp(WORD virtualKeyCode)
{
	if (cNumRead == 0) return false;

	for (int i = 0; i < cNumRead; i++)
	{
		if (irInBuf[i].EventType != KEY_EVENT) continue;

		if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == virtualKeyCode &&
			irInBuf[i].Event.KeyEvent.bKeyDown == FALSE) {
			return true;
		}
	}
	return false;
}


void Input::KeyEventProc(KEY_EVENT_RECORD ker)
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

void Input::MouseEventProc(MOUSE_EVENT_RECORD mer)
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

void Input::ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	Borland::gotoxy(0, 13);
	printf("%s\r", blankChars);
	printf("Resize event: ");
	printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
	Borland::gotoxy(0, 0);
}