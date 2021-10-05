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

// singleton 패턴
// object instance가 단 하나만 존재
// 1. 생성자 함수에 외부에 숨겨라 -> 생성자 함수를 private 함수로
// 2. 해당 object instance가 저장될 수 있도록 클래스내에 선언해야 함
//    static ClassName* instance 선언 그리고 외부에 해당 static 포인터 변수 초기화
// 3. static ClassName* GetInstance() 를 public으로 선언하고 정의


// Generic Programming: C#, Java
// Template Programming: C++

template <typename T>
void print(T input)
{
	std::cout << input;
}






class Screen;
class GameObject;
class Input;

class Screen {
private:
	int		width; // visible width
	int		height;
	int		size;
	char*	canvas;

	// constructor (생성자 함수) 메모리공간상에 적재되는 순간 호출되는
	Screen(int width = 10, int height = 10)
		: width(width), height(height), canvas(new char[(width + 1) * height])
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

	static Screen* Instance;


public:

	static Screen* GetInstance() {
		if (Instance == nullptr) {
			Instance = new Screen(20, 10);
		}
		return Instance;
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

Screen* Screen::Instance = nullptr;


class Input {
	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128];
	int counter;
	HANDLE hStdin;
	DWORD fdwSaveOldMode;
	char blankChars[80];

	void errorExit(const char*);
	void keyEventProc(KEY_EVENT_RECORD);
	void mouseEventProc(MOUSE_EVENT_RECORD);
	void resizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

	static Input* Instance;

	Input()
	{
		memset(blankChars, ' ', 80);
		blankChars[79] = '\0';

		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		if (hStdin == INVALID_HANDLE_VALUE)
			errorExit("GetStdHandle");
		if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
			errorExit("GetConsoleMode");
		/*
			   Step-1:
			   Disable 'Quick Edit Mode' option programmatically
		 */
		fdwMode = ENABLE_EXTENDED_FLAGS;
		if (!SetConsoleMode(hStdin, fdwMode))
			errorExit("SetConsoleMode");
		/*
		   Step-2:
		   Enable the window and mouse input events,
		   after you have already applied that 'ENABLE_EXTENDED_FLAGS'
		   to disable 'Quick Edit Mode'
		*/
		fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
		if (!SetConsoleMode(hStdin, fdwMode))
			errorExit("SetConsoleMode");

	}

	~Input() {
		SetConsoleMode(hStdin, fdwSaveOldMode);
	}

public:

	static Input* GetInstance()
	{
		if (Instance == nullptr) {
			Instance = new Input();
		}
		return Instance;
	}

	void readInputs() 
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
			errorExit("ReadConsoleInput");
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
	bool getKeyDown(WORD virtualKeyCode);
	bool getKey(WORD virtualKeyCode);
	bool getKeyUp(WORD virtualKeyCode);
};

Input* Input::Instance = nullptr;


class GameObject
{
private:
	char		face[20];
	Position	pos;
	Dimension	dim;
	Screen*		screen;
	Input*		input;

public:

	GameObject(const char* face, const Position& pos, const Dimension& dim)
		: pos(pos), screen( Screen::GetInstance() ), dim(dim), input( Input::GetInstance() )
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
		if (input->getKey(VK_LEFT)) {
			if (pos.x <= 0) return;
			pos.x = (pos.x - 1) % (screen->getWidth());
		}
		if (input->getKey(VK_RIGHT)) {
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
	Screen* screen = Screen::GetInstance();
	Input*  input = Input::GetInstance();
	Position pos{ 1, 2 };
	const char shape[] = "**    **     **";
	Dimension sz{ (int)strlen(shape), 1 };
	GameObject one{ shape, pos,  sz };
	
	int a = 30;
	double f = 30.4;
	print<double>(f);
	
	// Get the standard inp
	
	bool isLooping = true;
	while (isLooping) {
	
		screen->clear();
		one.draw();

		input->readInputs();

		one.update();

		screen->render();
		Sleep(100);
		
	}
	printf("\nGame Over\n");

	return 0;
}

void Input::errorExit(const char* lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);
	
	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

bool Input::getKeyDown(WORD virtualKeyCode)
{
	// TODO: NOT FULLY IMPLEMENTED YET
	return getKey(virtualKeyCode); 
}

bool Input::getKey(WORD virtualKeyCode)
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

bool Input::getKeyUp(WORD virtualKeyCode)
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


void Input::keyEventProc(KEY_EVENT_RECORD ker)
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

void Input::mouseEventProc(MOUSE_EVENT_RECORD mer)
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

void Input::resizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	Borland::gotoxy(0, 13);
	printf("%s\r", blankChars);
	printf("Resize event: ");
	printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
	Borland::gotoxy(0, 0);
}