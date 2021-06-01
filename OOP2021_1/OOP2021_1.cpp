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

struct Screen;
struct Player;
struct Enemy;
struct Bullet;


/*
  C (재사용하기 위해서 a part of ....)
  --> C++ (상속: 재사용하기 위한 관점 ---> 다중 상속)
  --> 상속: is a 관계 표현하는 것이다!!! 단일 상속
  --> java (90중반 SUN james gosling: Object-oriented Language) -> oracle (java)

	  struct Player extends GameObject { ... }
		---> google (android -> mobile platform) -----------------------> kotlin
	  --> MS C# : struct Player : GameObject { ... }

   C++: class ScannerPrinter : public Printer, public IScanner

   java: class ScannerPrinter extends Printer, implements IScanner
   C#: class ScannerPrinter : Printer, IScanner
*/

struct Screen {
	int		size;
	char*	canvas;

	// constructor (생성자 함수) 메모리공간상에 적재되는 순간 호출되는
	Screen(unsigned int size)
	{
		if (size == 0) size = 80;
		this->size = size;
		canvas = new char[size+1];
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
	bool isInRange(Bullet* bullet);

	// destructor (소멸자 함수) 메모리공간상에서 없어지는 순간 호출되는 함수
	~Screen()
	{
		delete[] canvas;
		canvas = nullptr;
		size = 0;
	}
};
struct GameObject
{
	char	face[20];
	int		pos;
	int		direction;
	Screen* screen;
	GameObject** gameObjects;

	GameObject(GameObject** gameObjects, Screen* screen, const char* face, int pos, int direction, const char* type) 
		: pos(pos), direction(direction), screen(screen), gameObjects(gameObjects)
	{
		setFace(face);
		strcpy(this->type, type);
	}

	void move(int direction)
	{
		direction == directionToRight ? pos++: pos--;
	}
	void move()
	{
		(direction == directionToLeft) ? --pos : ++pos;
	}
	void draw()
	{
		screen->draw(pos, face);
	}
	void update() {}

	int getPos() { return pos; } // getter function
	void setPos(int pos) { this->pos = pos; } // setter function

	int getDirection() { return direction;  }
	void setDirection(int direction) { this->direction = direction; }

	const char* getFace() { return face;  }
	void setFace(const char* face) { strcpy(this->face, face); }

	bool equal(const char* type)
	{
		return strcmp(this->type, type) == 0;
	}

	Screen* getScreen() { return screen; }
	GameObject** getGameObjects() { return gameObjects; }
};
struct Player : public GameObject {
	int			nRemaining;
	char		originalFace[20];
	
	Player(GameObject** gameObjects, Screen* screen, const char* face, int pos);
	void fire();
	Bullet* find_unused_bullet();
	Enemy* find_closest_enemy();
	void draw();
	void update();
	void onEnemyHit()
	{
		setFace("\\(^_^)/");
		nRemaining = 30;
	}
};
struct Enemy : public GameObject {
	int		nRemaining;
	int     nMovementInterval;
	float   fPos;
	char	originalFace[20];

	Enemy(GameObject** gameObjects, Screen* screen, const char* face, int pos)
		: GameObject(gameObjects, screen, face, pos, directionToLeft, "enemy"), 
		nRemaining(0), nMovementInterval(1), fPos(pos)
	{	
		strcpy(originalFace, face);
	}

	void update()
	{
		int movement = rand() % 3 - 1;
		fPos += movement * 0.3f;
		setPos((int)fPos);

		if (nRemaining == 0) return;
		--nRemaining;
		if (nRemaining == 0) setFace(originalFace);
	}
	bool isHit(Bullet* bullet);
	void onHit()
	{
		setFace("(T_T)");
		nRemaining = 10;
	}
};
struct Bullet : public GameObject {
	bool	isReady;
	
	Bullet(GameObject** gameObjects, Screen* screen)
		: GameObject(gameObjects, screen, "-", 0, directionToLeft, "bullet"),
		isReady{ true }
	{}

	~Bullet() {}
	void setFire(Player* player, Enemy* enemy)
	{
		isReady = false; // inUse

		// direction 설정
		int enemy_pos = enemy->getPos();
		int player_pos = player->getPos();
		const char* player_face = player->getFace();

		setDirection(directionToLeft);
		if (player_pos < enemy_pos) setDirection(directionToRight);

		// bullet position 설정
		setPos(player_pos);
		if (getDirection() == directionToRight) 
			setPos( getPos() + (strlen(player_face) - 1));
	}
	
	void makeReady()
	{
		isReady = true;
	}

	void draw()
	{
		if (isReady == true) return;
		GameObject::draw();
	}
	void update()
	{
		if (isReady == true) return;

		move();
		GameObject** gameObjects = getGameObjects();
		Player* player = nullptr;
		for (int i = 0; i < 83; i++)
		{
			GameObject* obj = gameObjects[i];
			if (obj->equal("player")) {
				player = (Player*)obj;
			}
		}

		for (int i = 0; player != nullptr && i < 83; i++)
		{
			GameObject* obj = gameObjects[i];
			if (obj->equal("enemy")) {
				Enemy* enemy = (Enemy*)obj;
				if (enemy->isHit(this))
				{ // 적이 총알을 맞았을 때
					enemy->onHit();
					player->onEnemyHit();
					makeReady();
				}
			}
		}
		Screen* screen = getScreen();
		if (!screen->isInRange(this)) makeReady();
	}
	
	bool isAvailable() { return isReady; }
};


// forward declaration 전방위 선언
// function prototype declaration 함수 원형 선언
bool Screen::isInRange(Bullet* bullet)
{
	int bullet_pos = bullet->getPos();
	return bullet_pos >= 0 && bullet_pos < size;
}
Player::Player(GameObject** gameObjects, Screen* screen, const char* face, int pos)
	: GameObject(gameObjects, screen, face, pos, directionToRight, "player"), nRemaining(0)
{
	strcpy(originalFace, face);
}
void Player::fire()
{
	Bullet* bullet = find_unused_bullet();
	if (bullet == nullptr) return;
	Enemy* enemy = find_closest_enemy();
	if (enemy == nullptr) return;
	bullet->setFire(this, enemy);
}
Bullet* Player::find_unused_bullet()
{
	GameObject** gameObjects = getGameObjects();
	for (int i = 0; i < 83; i++)
	{
		GameObject* obj = gameObjects[i];
		if (obj->equal("bullet") == false) continue;
		Bullet* bullet = (Bullet*)obj;
		if (bullet->isAvailable() == true) return bullet;
	}
	return nullptr;
}
Enemy* Player::find_closest_enemy() 
{
	GameObject** gameObjects = getGameObjects();
	Enemy* closest = nullptr;
	for (int i = 0; i < 83; i++) {
		GameObject* obj = gameObjects[i];
		if (obj->equal("enemy") == false) continue;
		
		// obj->equal("enemy") == true
		Enemy* enemy = (Enemy*)obj;
		if (closest == nullptr) {
			closest = enemy;
			continue;
		}
		// closest != nullptr
		int player_pos = getPos();
		int enemy_pos = enemy->getPos();
		int closest_pos = closest->getPos();
		if (abs(player_pos - enemy_pos) < abs(player_pos - closest_pos)) {
			closest = enemy;
		}
	}
	return closest;
}
void Player::draw()
{
	GameObject::draw();
}
void Player::update()
{
	if (nRemaining == 0) return;
	--nRemaining;
	if (nRemaining == 0) setFace(originalFace);
}
bool Enemy::isHit(Bullet* bullet)
{
	int bullet_direction = bullet->getDirection();
	int bullet_pos = bullet->getPos();
	return bullet_pos >= getPos() && bullet_pos < getPos() + strlen(getFace()) - 1;
}

int main()
{	
	int major;
	int minor;

	Screen  screen(80);
	GameObject* gameObjects[80+1+2];

	gameObjects[0] = new Player(gameObjects, &screen, "(-_-)", 50);
	gameObjects[1] = new Enemy(gameObjects, &screen, "(`_#)", 10);
	gameObjects[2] = new Enemy(gameObjects, &screen, "(*_*)", 30);
	for (int i = 0; i < 80; i++)
		gameObjects[i+3] = new Bullet(gameObjects, &screen);

	// game loop

	bool isLooping = true;
	while (isLooping) {
		screen.clear();		  		

		for (int i = 0; i < 83; i++) {
			GameObject* obj = gameObjects[i];

			Player* player = dynamic_cast<Player *>(obj);
			if (player != nullptr) {
				player->update();
				continue;
			}
			// player == nullptr
			Enemy* enemy = dynamic_cast<Enemy *>(obj);
			if (enemy != nullptr) {
				enemy->update();
				continue;
			}
			// enemy == nullptr
			Bullet* bullet = dynamic_cast<Bullet *>(obj);
			if (bullet != nullptr) {
				bullet->update();
			}
		}

		for (int i = 0; i < 83; i++) {
			GameObject* obj = gameObjects[i];
			if (obj->equal("player")) {
				Player* player = (Player*)obj;
				player->draw();
			}
			else if (obj->equal("enemy")) {
				Enemy* enemy = (Enemy*)obj;
				enemy->draw();
			}
			else if (obj->equal("bullet")) {
				Bullet* bullet = (Bullet*)obj;
				bullet->draw();
			}
		}
		
		screen.render();
		Sleep(100);

		if (!_kbhit()) continue;

		major = _getch();
		switch (major) {
		case 'q':
			isLooping = false;
			break;

		case ' ':
			(static_cast<Player *>(gameObjects[0]))->fire();
			break;
		case 224: // arrow key, function key pressed
			minor = _getch();
			switch (minor) {
			case 75: // left
				gameObjects[0]->move(directionToLeft); // 				
				break;
			case 77: // right
				gameObjects[0]->move(directionToRight);
				break;
			case 72: // up
				gameObjects[1]->move(directionToLeft);
				break;
			case 80: // down
				gameObjects[1]->move(directionToRight);
				break;
			}
			break;
		}
	}
	printf("\nGame Over\n");

	for (int i = 0; i < 83; i++)
		delete gameObjects[i];

	return 0;
}