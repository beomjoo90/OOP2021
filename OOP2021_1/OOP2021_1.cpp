#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <cstring> // string.h
#include <cstdlib> // stdlib.h
#include <string> // c++ string class
#include <Windows.h>

using namespace std;

int main()
{
	int bulletPos = 0;
	const int screenSize = 80;
	char screen[screenSize + 1];

	while (true) {

		memset(screen, ' ', screenSize);

		screen[bulletPos] = '-';
		screen[screenSize] = '\0';
		printf("%s\r", screen);

		bulletPos++;
		if (bulletPos >= screenSize) break;

		Sleep(100);
	}
	printf("\nGame Over\n");
	
	return 0;
}