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

const int directionToLeft = 0;
const int directionToRight = 1;
const int screenSize = 80;

void clear(char* screen)
{
	memset(screen, ' ', screenSize);		 
}
void draw(char* screen, int pos, const char* face)
{
	strncpy(&screen[pos], face, strlen(face));
}
void draw(char* screen, int pos, char face)
{
	if (pos < 0 || pos >= screenSize) return;
	screen[pos] = face;
}
void render(char* screen)
{
	screen[screenSize] = '\0';  // render screen
	printf("%s\r", screen);
}

void moveLeft(int* position)
{
	--(*position);
}
void moveRight(int* position)
{
	++(*position);
}
void update(char* face, int* nRemaining, const char* target)
{
	if (*nRemaining > 0) {
		--(*nRemaining);
		if ( (*nRemaining) == 0) { // 1초가 지났을 때
			strcpy(face, target);
		}
	}
}

void initBullet(bool* isReady, int* bulletPosition, int* direction)
{
	*isReady = true;
	*bulletPosition = 0;
	*direction = directionToRight;
}
void initBullets(bool* areReady, int* bulletPositions, int* directions, const int nBullets)
{	
	for (int i = 0; i < nBullets; i++)
	{
		initBullet(&areReady[i], &bulletPositions[i], &directions[i]);
	}
}
void updateBullets(bool* areReady, int* bulletPositions, int* directions, const int nBullets,
		char* playerFace, int* nPlayerRemaining,
		char* enemyFace, int enemyPos, int* nEnemyRemaining	) {
	// update all bullets
	for (int i = 0; i < nBullets; i++)
	{
		if (areReady[i] == true) continue;

		// isReady[i] == false
		directions[i] == directionToRight ? bulletPositions[i]++ : bulletPositions[i]--; // move bullet automatically
		if ((directions[i] == directionToLeft && enemyPos + strlen(enemyFace) - 1 == bulletPositions[i])
			|| (directions[i] == directionToRight && enemyPos == bulletPositions[i])) { // 적이 총알을 맞았을 때
			strcpy(enemyFace, "(T_T)");
			*nEnemyRemaining = 10;
			strcpy(playerFace, "\\(^_^)/");
			*nPlayerRemaining = 30;
			areReady[i] = true;
		}

		if (bulletPositions[i] < 0 || bulletPositions[i] >= screenSize) {
			areReady[i] = true;
		}
	}
}
int findUnusedBullet(bool* areReady, const int nBullets)
{
	for (int i = 0; i < nBullets; i++) 
	{
		if (areReady[i] == true) return i;
	}
	
	return -1;
}
void fire(bool* areReady, int* bulletPositions, int* directions, const int nBullets,
	int playerPos, const char* playerFace,
	int enemyPos )
{
	int i = findUnusedBullet(areReady, nBullets);
	if (i == -1) return;

	areReady[i] = false; // 사용중으로 표시
	if (playerPos < enemyPos) directions[i] = directionToRight;
	else directions[i] = directionToLeft;

	if (directions[i] == directionToRight) bulletPositions[i] = playerPos + strlen(playerFace) - 1;
	else bulletPositions[i] = playerPos;
}

int main()
{	
	char screen[screenSize + 1]; // 0 .. 80 : 81

	char playerFace[20]{ "(-_-)" };
	int playerPos = 50;
	int nPlayerRemaining = 0;

	char enemyFace[20]{ "(`_#)" };
	int enemyPos = 10;
	int nEnemyRemaining = 0;

	const int nBullets = screenSize;
	bool areReady[nBullets];
	int bulletPositions[nBullets];
	int directions[nBullets];

	int major;
	int minor;

	initBullets(areReady, bulletPositions, directions, nBullets);

	// game loop
	while (true) {
		clear(screen);		  		

		if (_kbhit() ) {
			major = _getch();
			switch (major) {
				
			case ' ':
				fire(areReady, bulletPositions, directions, nBullets, playerPos, playerFace, enemyPos);
				break;
			case 224: // arrow key, function key pressed
				minor = _getch();
				switch (minor) {
				case 75: // left
					moveLeft( &playerPos );					
					break;
				case 77: // right
					moveRight( &playerPos );
					break;
				case 72: // up
					moveLeft(&enemyPos);
					break;
				case 80: // down
					moveRight( &enemyPos );
					break;
				}
				break;
			}
		}

		update(playerFace, &nPlayerRemaining, "(-_-)");
		update(enemyFace, &nEnemyRemaining, "(`_#)");
		updateBullets(areReady, bulletPositions, directions, nBullets,
			playerFace, &nPlayerRemaining,
			enemyFace, enemyPos, &nEnemyRemaining);

		draw(screen, playerPos, playerFace);
		draw(screen, enemyPos, enemyFace);
		for (int i = 0; i < nBullets; i++)
		{
			if (areReady[i] == true) continue;
			draw(screen, bulletPositions[i], '-');
		}
		
		render(screen);

		Sleep(100);
	}
	printf("\nGame Over\n");
	
	return 0;
}