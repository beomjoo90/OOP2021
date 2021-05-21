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
struct Bullets;


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

	GameObject(const char* face, int pos, int direction) 
		: pos(pos), direction(direction)
	{
		setFace(face);
	}

	void move(int direction)
	{
		direction == directionToRight ? pos++: pos--;
	}
	void move()
	{
		(direction == directionToLeft) ? --pos : ++pos;
	}
	void draw(Screen* screen)
	{
		screen->draw(pos, face);
	}

	int getPos() { return pos; } // getter function
	void setPos(int pos) { this->pos = pos; } // setter function

	int getDirection() { return direction;  }
	void setDirection(int direction) { this->direction = direction; }

	const char* getFace() { return face;  }
	void setFace(const char* face) { strcpy(this->face, face); }

	~GameObject() {}
};
struct Player : public GameObject {
	int		nRemaining;
	int		nBullets; // 80
	Bullet*	bullets;
	
	Player(const char* face, int pos);
	~Player();		
	void fire(Enemy* enemy);
	Bullet* find_unused_bullet();
	void draw(Screen* screen);
	void update(const char* face);
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

	Enemy(const char* face, int pos)
		: GameObject(face, pos, directionToLeft), nRemaining(0), nMovementInterval(1), fPos(pos)
	{}
	void update(const char* face)
	{
		int movement = rand() % 3 - 1;
		fPos += movement * 0.3f;
		setPos((int)fPos);

		if (nRemaining == 0) return;
		--nRemaining;
		if (nRemaining == 0) setFace(face);
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
	
	Bullet() : GameObject("-", 0, directionToLeft), isReady{true}
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

	void draw(Screen* screen)
	{
		if (isReady == true) return;
		GameObject::draw(screen);
	}
	void update(Player* player, Enemy* enemy, Screen* screen)
	{
		if (isReady == true) return;

		move();
		if (enemy->isHit(this))
		{ // 적이 총알을 맞았을 때
			enemy->onHit();
			player->onEnemyHit();
			makeReady();
		}
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
Player::Player(const char* face, int pos)
	: GameObject(face, pos, directionToRight), nRemaining(0),
	nBullets(80), bullets{ new Bullet[nBullets] }
{}
Player::~Player()
{
	delete[] bullets;
	bullets = nullptr;
	nBullets = 0;
}
void Player::fire(Enemy* enemy)
{
	Bullet* bullet = find_unused_bullet();
	if (bullet == nullptr) return;
	bullet->setFire(this, enemy);
}
Bullet* Player::find_unused_bullet()
{
	for (int i = 0; i < nBullets; i++)
	{
		Bullet* bullet = &bullets[i];
		if (bullet->isAvailable() == true) return bullet;
	}
	return nullptr;
}
void Player::draw(Screen* screen)
{
	GameObject::draw(screen);
	for (int i = 0; i < nBullets; i++)
	{
		Bullet* bullet = &bullets[i];
		bullet->draw(screen);
	}
}
void Player::update(const char* face)
{
	for (int i = 0; i < nBullets; i++)
	{
		Bullet* bullet = &bullets[i];
		//bullet->update(this, enemy, screen);
	}

	if (nRemaining == 0) return;
	--nRemaining;
	if (nRemaining == 0) setFace(face);
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
	Player*	player = new Player( "(-_-)", 50);
	Enemy * enemy = new Enemy( "(`_#)", 10);
	Enemy*	enemy2 = new Enemy("(*_*)", 30);

	// game loop

	bool isLooping = true;
	while (isLooping) {
		screen.clear();		  		

		player->update("(-_-)");
		enemy->update( "(`_#)");
		enemy2->update( "(*_*)");


		player->draw(&screen);
		enemy->draw(&screen);	
		enemy2->draw(&screen);
		
		screen.render();
		Sleep(100);

		if (!_kbhit()) continue;

		major = _getch();
		switch (major) {
		case 'q':
			isLooping = false;
			break;

		case ' ':
			player->fire(enemy2);
			break;
		case 224: // arrow key, function key pressed
			minor = _getch();
			switch (minor) {
			case 75: // left
				player->move(directionToLeft); // 				
				break;
			case 77: // right
				player->move(directionToRight);
				break;
			case 72: // up
				enemy->move(directionToLeft);
				break;
			case 80: // down
				enemy->move(directionToRight);
				break;
			}
			break;
		}
	}
	printf("\nGame Over\n");

	delete enemy2;
	delete enemy;
	delete player;

	return 0;
}