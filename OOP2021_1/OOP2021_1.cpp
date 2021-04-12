#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <conio.h> // console io
#include <cstring> // string.h
#include <cstdlib> // stdlib.h
#include <string> // c++ string class
#include <Windows.h>

using namespace std;

int main()
{	
	int bulletPos = 0;
	const int screenSize = 80;
	char screen[screenSize + 1]; // 0 .. 80 : 81
	char playerFace[20]{ "(^_^)" };
	int playerPos = 10;

	int key;


	// game loop
	while (true) {

		if (bulletPos < 0 || bulletPos >= screenSize) break;

		memset(screen, ' ', screenSize); // clear screen

		strncpy(&screen[playerPos], playerFace, strlen(playerFace) ) ; // draw player

		if (_kbhit() ) {
			key = _getch();
			printf("\nkey %c %d\n", key, key);
		}

		screen[screenSize] = '\0';  // render screen
		printf("%s\r", screen);

		Sleep(100);
	}
	printf("\nGame Over\n");
	
	return 0;
}