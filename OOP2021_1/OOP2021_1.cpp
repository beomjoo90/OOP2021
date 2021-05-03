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

const int directionToLeft = 0;
const int directionToRight = 1;

struct Screen {
	int		size;
	char*	canvas;

	// constructor (생성자 함수) 메모리공간상에 적재되는 순간 호출되는
	Screen(unsigned int size)
	{
		if (size == 0) size = 80;
		this->size = size;
		canvas = (char*)malloc(sizeof(char)*(size + 1));
	}
	void clear()
	{
		memset(canvas, ' ', size);
	}
	void draw(int pos, const char* face)
	{
		strncpy(&(canvas[pos]), face, strlen(face));
	}
	void draw(int pos, char face)
	{
		if (pos < 0 || pos >= size) return;
		canvas[pos] = face;
	}
	void render()
	{
		canvas[size] = '\0';  // render screen
		printf("%s\r", canvas);
	}
	bool isInRange(Bullet* bullet)
	{
		int bullet_pos = bullet->getPos(bullet);
		return bullet_pos >= 0 && bullet_pos <size;
	}

	// destructor (소멸자 함수) 메모리공간상에서 없어지는 순간 호출되는 함수
	~Screen()
	{
		free(canvas);
		canvas = nullptr;
		size = 0;
	}
};
struct Player {
	char	face[20];
	int		pos;
	int		nRemaining;
};
struct Enemy {
	char	face[20];
	int		pos;
	int		nRemaining;
};
struct Bullet {
	bool	isReady;
	int		pos;
	int		direction;
};
struct Bullets {
	int		nBullets; // 80
	Bullet*	bullets;
};


// forward declaration 전방위 선언
// function prototype declaration 함수 원형 선언

extern int player_getPos(Player*);
extern const char* player_getFace(Player*);

extern int enemy_getPos(Enemy*);

extern int bullet_getPos(Bullet*);
extern int bullet_getDirection(Bullet* bullet);

extern void bullet_setFire(Bullet* bullet, Player* player, Enemy* enemy);
extern int bullets_find_unused_bullet(Bullets* bullets);



void player_init(Player* player, const char* face, int pos)
{
	strcpy(player->face, face);
	player->pos = pos;
	player->nRemaining = 0;
}
void player_fire(Player* player, Bullets* bullets, Enemy* enemy)
{
	int i = bullets_find_unused_bullet(bullets);
	if (i == -1) return;
	bullet_setFire(&(bullets->bullets[i]), player, enemy);
}
void player_move(Player* player, int direction)
{
	(direction == directionToLeft) ? --(player->pos) : ++(player->pos);
}
void player_draw(Player* player, Screen* screen)
{
	screen->draw(player->pos, player->face);
}
void player_update(Player* player, const char* face)
{
	if (player->nRemaining == 0) return;
	--(player->nRemaining);
	if (player->nRemaining == 0) strcpy(player->face, face);
}
void player_onEnemyHit(Player* player)
{
	strcpy(player->face, "\\(^_^)/");
	player->nRemaining = 30;
}
int  player_getPos(Player* player)
{
	return player->pos;
}
const char* player_getFace(Player* player)
{
	return player->face;
}

void enemy_init(Enemy* enemy, const char* face, int pos)
{
	strcpy(enemy->face, face);
	enemy->pos = pos;
	enemy->nRemaining = 0;
}
void enemy_move(Enemy* enemy, int direction)
{
	direction == directionToLeft ? --(enemy->pos) : ++(enemy->pos);
}
void enemy_draw(Enemy* enemy, Screen* screen)
{
	screen_draw(screen, enemy->pos, enemy->face);
}
void enemy_update(Enemy* enemy, const char* face)
{
	if (enemy->nRemaining == 0) return;

	--(enemy->nRemaining);
	if (enemy->nRemaining == 0) strcpy(enemy->face, face);
}
bool enemy_isHit(Enemy* enemy, Bullet* bullet)
{
	int bullet_direction = bullet_getDirection(bullet);
	int bullet_pos = bullet_getPos(bullet);
	return (
		(bullet_direction == directionToLeft && enemy->pos + strlen(enemy->face) - 1 == bullet_pos)
		|| (bullet_direction == directionToRight && enemy->pos == bullet_pos));
}
void enemy_onHit(Enemy* enemy)
{
	strcpy(enemy->face, "(T_T)");
	enemy->nRemaining = 10;
}
int  enemy_getPos(Enemy* enemy)
{
	return enemy->pos;
}

void bullet_init(Bullet* bullet) 
{
	bullet->isReady = true;
	bullet->pos = 0;
	bullet->direction = directionToLeft;
}
void bullet_setFire(Bullet* bullet, Player* player, Enemy* enemy)
{
	bullet->isReady = false; // inUse

	// direction 설정
	int enemy_pos = enemy_getPos(enemy);
	int player_pos = player_getPos(player);
	const char* player_face = player_getFace(player);

	bullet->direction = directionToLeft;
	if (player_pos < enemy_pos) bullet->direction = directionToRight;

	// bullet position 설정
	bullet->pos = player_pos;
	if (bullet->direction == directionToRight) bullet->pos += (strlen(player_face) - 1);
}
void bullet_move(Bullet* bullet)
{
	(bullet->direction == directionToLeft) ? --(bullet->pos) : ++(bullet->pos);
}
void bullet_draw(Bullet* bullet, Screen* screen)
{
	if (bullet->isReady == true) return;
	screen_draw(screen, bullet->pos, '-');
}
void bullet_reuse(Bullet* bullet)
{
	bullet->isReady = true;
}
void bullet_update(Bullet* bullet, Player* player, Enemy* enemy, Screen* screen)
{
	if (bullet == nullptr) return;
	if (bullet->isReady == true) return;

	bullet_move(bullet);
	if (enemy_isHit(enemy, bullet))
	{ // 적이 총알을 맞았을 때
		enemy_onHit(enemy);
		player_onEnemyHit(player);
		bullet_reuse(bullet);
	}
	if (!screen_isInRange(screen, bullet)) bullet_reuse(bullet);
}
int  bullet_getPos(Bullet* bullet) // function definition
{
	return bullet->pos;
}
int  bullet_getDirection(Bullet* bullet)
{
	return bullet->direction;
}

void bullets_init(Bullets* bullets, unsigned int nBullets)
{
	if (nBullets == 0) nBullets = 80;
	bullets->nBullets = nBullets;
	bullets->bullets = (Bullet *)malloc(sizeof(Bullet) * nBullets);
	for (int i = 0; i < bullets->nBullets; i++)
		bullet_init( &(bullets->bullets[i]) );
}
void bullets_draw(Bullets* bullets, Screen* screen)
{
	for (int i = 0; i < bullets->nBullets; i++)
	{
		bullet_draw( &(bullets->bullets[i]), screen);
	}
}
void bullets_update(Bullets* bullets, Player* player, Enemy* enemy, Screen* screen)
{
	for (int i = 0; i < bullets->nBullets; i++)
	{	
		bullet_update(&(bullets->bullets[i]), player, enemy, screen);
	}
}
int  bullets_find_unused_bullet(Bullets* bullets)
{
	for (int i = 0; i < bullets->nBullets; i++)
	{
		Bullet* bullet = &bullets->bullets[i];
		if (bullet->isReady == true) return i;
	}
	return -1;
}
void bullets_deinit(Bullets* bullets)
{
	free(bullets->bullets);
	bullets->bullets = nullptr;
	bullets->nBullets = 0;
}

int main()
{	
	int major;
	int minor;

	Screen	screen(80);  
	Player	player;
	Enemy	enemy;
	Bullets bullets;

	player_init(&player, "(-_-)", 50);
	enemy_init(&enemy, "(`_#)", 10);
	bullets_init(&bullets, 80);

	// game loop

	bool isLooping = true;
	while (isLooping) {
		screen.clear();		  		

		player_update(&player, "(-_-)");
		enemy_update(&enemy, "(`_#)");
		bullets_update(&bullets, &player, &enemy, &screen);

		player_draw(&player, &screen);
		enemy_draw(&enemy, &screen);		
		bullets_draw(&bullets, &screen);
		
		screen.render();
		Sleep(100);

		if (!_kbhit()) continue;

		major = _getch();
		switch (major) {
		case 'q':
			isLooping = false;
			break;

		case ' ':
			player_fire(&player, &bullets, &enemy);
			break;
		case 224: // arrow key, function key pressed
			minor = _getch();
			switch (minor) {
			case 75: // left
				player_move(&player, directionToLeft); // 				
				break;
			case 77: // right
				player_move(&player, directionToRight);
				break;
			case 72: // up
				enemy_move(&enemy, directionToLeft);
				break;
			case 80: // down
				enemy_move(&enemy, directionToRight);
				break;
			}
			break;
		}
	}
	printf("\nGame Over\n");

	bullets_deinit(&bullets);
	
	return 0;
}