#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include "Position.h"
#include "Borland.h"
#include "Screen.h"

using namespace std;

enum class KeyCode {
	Space = 0,
	Left,
	Right,
	Up,
	Down,

	Esc,
	Enter,

	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
};

class Input
{
	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128];
	int counter;
	HANDLE hStdin;
	DWORD fdwSaveOldMode;
	char blankChars[80];
	WORD virtualKeyCodes[128];

	bool keys[(int)KeyCode::Z + 1];

	bool mouseClicked;
	COORD mousePosition;

	void keyEventProc(KEY_EVENT_RECORD);
	void mouseEventProc(MOUSE_EVENT_RECORD);
	void resizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

	static Input* Instance;
	static vector<WORD> KeyCodeTable;

	Input() : keys{ false }
	{
		memset(blankChars, ' ', 80);
		blankChars[79] = '\0';

		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		if (hStdin == INVALID_HANDLE_VALUE)
			errorExit("GetStdHandle");
		FlushConsoleInputBuffer(hStdin);

		string mode = "mode con:cols=" + to_string((Screen::GetInstance())->getWidth() + 10);
		mode += " lines=" + to_string((Screen::GetInstance())->getHeight() + 10);
		std::system(mode.c_str());
		std::system("chcp 437");

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

	void errorExit(const char*);

	void readInputs()
	{
		if (!GetNumberOfConsoleInputEvents(hStdin, &cNumRead)) {
			cNumRead = 0;
			return;
		}
		if (cNumRead == 0) return;

		Borland::Gotoxy(0, 14);
		printf("number of inputs %d\n", cNumRead);

		if (!ReadConsoleInput(
			hStdin,      // input buffer handle
			irInBuf,     // buffer to read into
			128,         // size of read buffer
			&cNumRead)) // number of records read
			errorExit("ReadConsoleInput");
		// Dispatch the events to the appropriate handler.

		for (int i = 0; i < (int)cNumRead; i++)
		{
			switch (irInBuf[i].EventType)
			{
			case KEY_EVENT: // keyboard input
				keyEventProc(irInBuf[i].Event.KeyEvent);
				break;

			case MOUSE_EVENT: // mouse input
				mouseEventProc(irInBuf[i].Event.MouseEvent);
				mousePosition = irInBuf[i].Event.MouseEvent.dwMousePosition;
				break;

			case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
				resizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
				break;

			case FOCUS_EVENT:  // disregard focus events

			case MENU_EVENT:   // disregard menu events
				break;

			default:
				errorExit("Unknown event type");
				break;
			}
		}

		Borland::Gotoxy(0, 0);
	}
	bool getKeyDown(WORD virtualKeyCode) const;
	bool getKey(WORD virtualKeyCode) const;
	bool getKeyUp(WORD virtualKeyCode) const;
	bool getMouseButtonDown(unsigned short number);
	Position getMousePosition() const { return { mousePosition.X, mousePosition.Y }; }
};

