#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Screen.h"
#include "Input.h"
#include "Board.h"
#include "Map.h"


using namespace std;


// https://github.com/beomjoo90/OOP2021 , branch: 2학기

class Screen;
class GameObject;
class Input;
class Map;



int main()
{
	Screen* screen = Screen::GetInstance();
	Input* input = Input::GetInstance();

	vector<GameObject*> objs;
	auto map1 = new Map{ 10, 10, 10, { 0, 0 } };
	auto map2 = new Map{ 10, 10, 10, { 25, 0 } };
	
	objs.push_back(map1);
	Sleep(1000);
	objs.push_back(map2);
	
	// Get the standard inp
	
	while (1) {
		bool completed = true;
		for (auto obj : objs) {
			auto map = dynamic_cast<Map*>(obj);
			if (map == nullptr) continue;
			if (map->isGameOver() == false) {
				completed = false;
				break;
			}
		}
		if (completed == true) break;
	
		screen->clear();
		input->readInputs();

		for (auto obj : objs) obj->update();
		for (auto obj : objs) obj->draw();

		screen->render();

		Sleep(100);
	}

	while (objs.size())
	{
		auto obj = objs.back();
		objs.pop_back();
		delete obj;
	}

	return 0;
}
