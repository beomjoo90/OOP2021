#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Position.h"
#include "Input.h"
#if 0
#include "Score.h"
#include "Block.h"
#include "Panel.h"
#endif
#include "TetrisGame.h"


using namespace std;


// https://github.com/beomjoo90/OOP2021 , branch: 2학기

int main()
{
	auto screen = Screen::GetInstance();
	auto input = Input::GetInstance();
	auto tetris = new TetrisGame;

	while (tetris->isGameOver() == false) 
	{
		screen->clear();

		input->readInputs();

		tetris->update();

		screen->render();

		Sleep(100);
	}

	delete tetris;

	return 0;
}
