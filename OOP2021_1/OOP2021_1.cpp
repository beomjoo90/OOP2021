#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <conio.h> // console io
#include <cstring> // string.h
#include <cstdlib> // stdlib.h
#include <string> // c++ string class
#include <Windows.h>

// https://github.com/beomjoo90/OOP2021 , branch: 1학기

using namespace std;

int main()
{	
	const int directionToLeft = 0;
	const int directionToRight = 1;

	const int screenSize = 80;
	const int nBullets = 80;
	char screen[screenSize + 1]; // 0 .. 80 : 81

	char playerFace[20]{ "(-_-)" };
	int playerPos = 50;
	int nPlayerRemaining = 0;

	char enemyFace[20]{ "(`_#)" };
	int enemyPos = 10;
	int nEnemyRemaining = 0;

	bool isReady[nBullets];
	int bulletPos[nBullets];
	int direction[nBullets];

	int major;
	int minor;

	// initialize bullets
	for (int i=0; i < nBullets; i++)
	{
		isReady[i] = true;
		bulletPos[i] = 0;
		direction[i] = directionToRight;
	}

	// game loop
	while (true) {
		memset(screen, ' ', screenSize); // clear screen			   		

		// handle inputs
		if (_kbhit() ) {
			int i = 0;

			major = _getch();
			switch (major) {
				
			case ' ':
				// 총알들중 현재 사용중이지 않은 것을 찾아냄.
				for (; i < nBullets; i++)
				{
					if (isReady[i] == true) break;
				}
				// i == nBullets (not found) or i < nBullets (found)
				if (i == nBullets) break;

				// i < nBullets 즉 사용허지 않은 총알이 있습니다.
				isReady[i] = false; // make the ready bullet in used
				
				if (playerPos < enemyPos) direction[i] = directionToRight;
				else direction[i] = directionToLeft;

				if (direction[i] == directionToRight) bulletPos[i] = playerPos + strlen(playerFace)-1;
				else bulletPos[i] = playerPos;
				break;
			case 224: // arrow key, function key pressed
				minor = _getch();
				switch (minor) {
				case 75: // left
					playerPos--;
					break;
				case 77: // right
					playerPos++;
					break;
				case 72: // up
					enemyPos--;
					break;
				case 80: // down
					enemyPos++;
					break;
				}
				break;
			}
			///
		}

		// update
		// update player
		if (nPlayerRemaining > 0) {
			nPlayerRemaining--;
			if (nPlayerRemaining == 0) { // 1초가 지났을 때
				strcpy(playerFace, "(-_-)");
			}
		}

		// update enemy
		if (nEnemyRemaining > 0) {
			nEnemyRemaining--;
			if (nEnemyRemaining == 0) { // 1초가 지났을 때
				strcpy(enemyFace, "(`_#)");
			}
		}

		// update all bullets
		for (int i = 0; i < nBullets; i++)
		{
			if (isReady[i] == true) continue;

			// isReady[i] == false
			direction[i] == directionToRight ? bulletPos[i]++ : bulletPos[i]--; // move bullet automatically
			if ((direction[i] == directionToLeft && enemyPos + strlen(enemyFace) - 1 == bulletPos[i])
				|| (direction[i] == directionToRight && enemyPos == bulletPos[i])) { // 적이 총알을 맞았을 때
				strcpy(enemyFace, "(T_T)");
				nEnemyRemaining = 10;
				strcpy(playerFace, "\\(^_^)/");
				nPlayerRemaining = 30;
				isReady[i] = true;
			}

			if (bulletPos[i] < 0 || bulletPos[i] >= screenSize) {
				isReady[i] = true;
			}
		}

		// draw game objects
		strncpy(&screen[playerPos], playerFace, strlen(playerFace)); // draw player
		strncpy(&screen[enemyPos], enemyFace, strlen(enemyFace)); // draw enemy
		// draw all bullets in use
		for (int i = 0; i < nBullets; i++)
		{
			if (isReady[i] == true) continue;
			// isReady[i] == false
			if (bulletPos[i] >= 0 && bulletPos[i] < screenSize) {
				screen[bulletPos[i]] = '-'; // draw bullet
			}
		}

		screen[screenSize] = '\0';  // render screen
		printf("%s\r", screen);

		Sleep(100);
	}
	printf("\nGame Over\n");
	
	return 0;
}