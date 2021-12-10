#include "Input.h"
#include <vector>

Input* Input::Instance = nullptr;
std::vector<WORD> Input::KeyCodeTable{
		VK_SPACE, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		VK_ESCAPE, VK_RETURN,
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
		0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A
};


bool Input::getKeyDown(WORD virtualKeyCode) const
{
	// TODO: NOT FULLY IMPLEMENTED YET
	return getKey(virtualKeyCode);
}

bool Input::getKey(WORD virtualKeyCode) const
{
	if (cNumRead == 0) return false;

	for (int i = 0; i < (int)cNumRead; i++)
	{
		if (irInBuf[i].EventType != KEY_EVENT) continue;

		if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == virtualKeyCode &&
			irInBuf[i].Event.KeyEvent.bKeyDown == TRUE) {
			return true;
		}
	}
	return false;
}

bool Input::getKeyUp(WORD virtualKeyCode) const 
{
	if (cNumRead == 0) return false;

	for (int i = 0; i < (int)cNumRead; i++)
	{
		if (irInBuf[i].EventType != KEY_EVENT) continue;

		if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == virtualKeyCode &&
			irInBuf[i].Event.KeyEvent.bKeyDown == FALSE) {
			return true;
		}
	}
	return false;
}

bool Input::getMouseButtonDown(unsigned short number) 
{
	for (int i = 0; i < (int)cNumRead; i++) {
		if (irInBuf[i].EventType != MOUSE_EVENT) continue;

		auto& mouseEvent = irInBuf[i].Event.MouseEvent;
		if (mouseEvent.dwEventFlags != 0) continue;

		if ((number == 0 && mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) ||
			(number == 1 && mouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) ||
			(number == 2 &&
				(mouseEvent.dwButtonState == (FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED)
					|| mouseEvent.dwButtonState == FROM_LEFT_2ND_BUTTON_PRESSED))) {
			mousePosition = mouseEvent.dwMousePosition;
			return true;
		}
	}
	return false;
}

void Input::keyEventProc(KEY_EVENT_RECORD ker)
{
	Borland::Gotoxy(0, 11);
	printf("%s\r", blankChars);
	if (ker.bKeyDown) {

	}
	switch (ker.wVirtualKeyCode) {

	case VK_LBUTTON:
		break;
	case VK_BACK:
		break;
	case VK_RETURN:
		break;
	case VK_LEFT:
		break;
	case VK_UP:
		break;
	default:
		if (ker.wVirtualKeyCode >= 0x30 && ker.wVirtualKeyCode <= 0x39)
			printf("Key event: %c %d", ker.wVirtualKeyCode - 0x30 + '0', ker.wRepeatCount);
		else printf("Key event: %c %d", ker.wVirtualKeyCode - 0x41 + 'A', ker.wRepeatCount);
		break;
	}

	Borland::Gotoxy(0, 0);
}

void Input::mouseEventProc(MOUSE_EVENT_RECORD mer)
{
	Borland::Gotoxy(0, 12);
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
			printf("button press %x\n", mer.dwButtonState);
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
	Borland::Gotoxy(0, 0);
}

void Input::resizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	Borland::Gotoxy(0, 13);
	printf("%s\r", blankChars);
	printf("Resize event: ");
	printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
	Borland::Gotoxy(0, 0);
}

void Input::errorExit(const char* lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}
