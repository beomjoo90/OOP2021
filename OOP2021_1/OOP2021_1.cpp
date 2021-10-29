#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Position.h"
#include "Input.h"
#include "Text.h"
#include "TextInput.h"
#include "Block.h"
#include "Panel.h"


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

	auto panel = new Panel{ "", Position{3,3}, 10, 20, nullptr };
	new Block{ "\xdb  \xdb\xdb\xdb  \xdb", Position{5,0}, Dimension{ 3, 3}, panel };
	
	auto nextPanel = new Panel{ " Next", Position{20, 3}, 10, 5, nullptr };
	new Block{ "\xdb \xdb \xdb\xdb", Position{5, 1}, Dimension{ 2, 3 }, nextPanel, false };

	auto scorePanel = new Panel{ " Score", Position{20, 19}, 10, 4, nullptr };
	int value = 0;
	auto score = new TextInput{ value, Position{4, 2}, scorePanel };

	objs.push_back(panel);
	objs.push_back(nextPanel);
	objs.push_back(scorePanel);
	
	// Get the standard inp
	
	while (1) {
		screen->clear();
		input->readInputs();

		if (input->getKey(VK_ESCAPE)) break;
		if (input->getKey(VK_SPACE)) {
			value++;
			score->setValue(value);
		}

		for (auto object : objs) object->internalUpdate();
		for (auto object : objs) object->internalUpdatePos(false);
		for (auto object : objs) object->internalDraw();

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
