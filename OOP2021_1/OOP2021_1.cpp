#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <conio.h> // console io
#include <cstring> // string.h
#include <cstdlib> // stdlib.h
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include "Utils.h"

using namespace std;

// https://github.com/beomjoo90/OOP2021 , branch: 2학기

class Screen;
class GameObject;
class Input;
class Map;


class Screen {
private:
	int		width; // visible width
	int		height;
	int		size;
	char*	canvas;

	// constructor (생성자 함수) 메모리공간상에 적재되는 순간 호출되는
	Screen(int width = 80, int height = 20)
		: width(width), height(height), 
		canvas(new char[((size_t)width + 1) * height])
	{
		bool faultyInput = false;
		if (this->width <= 0) {
			this->width = 80;
			faultyInput = true;
		}
		if (this->height <= 0) {
			this->height = 20;
			faultyInput = true;
		}
		size = (this->width + 1) * this->height;
		if (faultyInput == true) {
			delete canvas;
			canvas = new char[size];
		}
	}
	// destructor (소멸자 함수) 메모리공간상에서 없어지는 순간 호출되는 함수
	virtual ~Screen()
	{
		if (canvas != nullptr)
			delete[] canvas;
		canvas = nullptr;
		width = 0; height = 0;
	}

	static Screen* Instance;

public:

	static Screen* GetInstance() {
		if (Instance == nullptr) {
			Instance = new Screen(40, 20);
		}
		return Instance;
	}

	int getWidth() const { return width; }

	void setWidth(int width) { this->width = width; }

	void clear() { memset(canvas, ' ', size); }

	int pos2Offset(const Position& pos) const { return (width + 1) * pos.y + pos.x; }

	void draw(const Position& pos, const char* shape, const Dimension& sz = Position{ 1, 1 } ) 
	{
		int offset = pos2Offset(pos);
		for (int h = 0; h < sz.y; h++)
			strncpy(&canvas[offset + (width + 1) * h], &shape[h * sz.x], sz.x);
	}

	void render()
	{
		Borland::gotoxy(0, 0);
		for (int h = 0; h < height; h++)
			canvas[(width + 1) * (h + 1) - 1] = '\n';
		canvas[size - 1] = '\0';
		printf("%s", canvas);
	}
	
};

Screen* Screen::Instance = nullptr;

enum class KeyCode {
	Space = 0,
	Left,
	Right,
	Up,
	Down,

	Esc,
	Enter,

	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
};

class Input {
	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128];
	int counter;
	HANDLE hStdin;
	DWORD fdwSaveOldMode;
	char blankChars[80];
	WORD virtualKeyCodes[128];

	bool keys[(int)KeyCode::Z + 1];

	bool mouseClicked;
	COORD mousePosition;
	
	void keyEventProc(KEY_EVENT_RECORD);
	void mouseEventProc(MOUSE_EVENT_RECORD);
	void resizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

	static Input* Instance;
	static std::vector<WORD> KeyCodeTable;

	Input() : keys{ false }
	{
		memset(blankChars, ' ', 80);
		blankChars[79] = '\0';		

		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		if (hStdin == INVALID_HANDLE_VALUE)
			errorExit("GetStdHandle");
		if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
			errorExit("GetConsoleMode");
		/*
			   Step-1:
			   Disable 'Quick Edit Mode' option programmatically
		 */
		fdwMode = ENABLE_EXTENDED_FLAGS;
		if (!SetConsoleMode(hStdin, fdwMode))
			errorExit("SetConsoleMode");
		/*
		   Step-2:
		   Enable the window and mouse input events,
		   after you have already applied that 'ENABLE_EXTENDED_FLAGS'
		   to disable 'Quick Edit Mode'
		*/
		fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
		if (!SetConsoleMode(hStdin, fdwMode))
			errorExit("SetConsoleMode");

	}

	~Input() {
		SetConsoleMode(hStdin, fdwSaveOldMode);
	}

public:

	static Input* GetInstance()
	{
		if (Instance == nullptr) {
			Instance = new Input();
		}
		return Instance;
	}

	void errorExit(const char*);

	void readInputs() 
	{
		if (!GetNumberOfConsoleInputEvents(hStdin, &cNumRead)) {
			cNumRead = 0;
			return;
		}
		if (cNumRead == 0) return;

		Borland::gotoxy(0, 14);
		printf("number of inputs %d\n", cNumRead);

		if (!ReadConsoleInput(
			hStdin,      // input buffer handle
			irInBuf,     // buffer to read into
			128,         // size of read buffer
			&cNumRead)) // number of records read
			errorExit("ReadConsoleInput");
		// Dispatch the events to the appropriate handler.

			for (int i = 0; i < cNumRead; i++)
			{
				switch (irInBuf[i].EventType)
				{
				case KEY_EVENT: // keyboard input
					keyEventProc(irInBuf[i].Event.KeyEvent);
					break;

				case MOUSE_EVENT: // mouse input
					mouseEventProc(irInBuf[i].Event.MouseEvent);
					mousePosition = irInBuf[i].Event.MouseEvent.dwMousePosition;
					break;

				case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
					resizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
					break;

				case FOCUS_EVENT:  // disregard focus events

				case MENU_EVENT:   // disregard menu events
					break;

				default:
					errorExit("Unknown event type");
					break;
				}
			}

		Borland::gotoxy(0, 0);
	}
	bool getKeyDown(WORD virtualKeyCode);
	bool getKey(WORD virtualKeyCode);
	bool getKeyUp(WORD virtualKeyCode);
	bool getMouseButtonDown(unsigned short number);
	Position getMousePosition() const {
		return { mousePosition.X, mousePosition.Y };
		// same as return Position{ mousePosition.X, mousePosition.Y }; // it uses temporary object.
	}
};

Input* Input::Instance = nullptr;
std::vector<WORD> Input::KeyCodeTable{
		VK_SPACE, VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		VK_ESCAPE, VK_RETURN,
		0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
		0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A
};

class GameObject
{
private:
	char*		shape;
	Position	pos;
	Dimension	dim;
	int			capacity;

protected:
	Screen*		screen;
	Input*		input;

public:

	GameObject(const char* face, const Position& pos, const Dimension& dim)
		: pos(pos), dim(dim), capacity((size_t)dim.x* dim.y),
		shape{new char[(size_t)dim.x * dim.y]},
		screen(Screen::GetInstance()), input(Input::GetInstance())
	{
		if (face == nullptr) return;
		strncpy(shape, face, min(strlen(face), capacity));
	}

	virtual ~GameObject() 
	{
		if (shape != nullptr)
			delete[] shape;
		shape = nullptr;
	}

	virtual void move(int pos) {}
	virtual void move(const Position& offset) {}
	virtual void move() {}

	virtual void draw() { screen->draw(pos, shape, dim); }

	virtual void update() {}

	Position getPos() const { return pos; } // getter function
	void setPos(const Position& pos) { this->pos = pos; } // setter function

	const char* getShape() const { return shape; }
	void setShape(const char* face) { strcpy(shape, face); }
	void setShape(char shape, int pos) 
	{
		if (pos < 0 || pos >= capacity) return;
		this->shape[pos] = shape;
	}

	Position screen2local(const Position& screenPosition) const {
		Position pos = getPos();
		return Position{ screenPosition.x - pos.x, screenPosition.y - pos.y };
	}
	Position local2screen(const Position& target) const {
		Position pos = getPos();
		return { pos.x + target.x, pos.y + target.y };
	}
};

class Map : public GameObject {

	enum class CellState {
		Closed = 0,
		Marked,
		Open,
	};

	// numberOfNeighboringMines of a Mined cellis assumed to be 9, which has no particular meaning
	static const int MineCell = 9; 

	static const char Closed = ' ';
	static const char Marked = '+';
	static const char Mined = '*';
	

	const int	width;
	const int	height;
	const int	size;	

	int*		numberOfNeighboringMines;
	CellState*	cellStates;

	int			numberOfTotalMines;
	bool		bGameOver;

	void initializeCells(int numberOfMines)
	{	
		// initialize cell states closed and its shape accordingly
		for (int cellNo = 0; cellNo < size; cellNo++) {
			cellStates[cellNo] = CellState::Closed;
			setShape(Closed, cellNo);
		}

		srand(time(nullptr));

		memset(numberOfNeighboringMines, 0, sizeof(int) * size);
		// create the given number of mines from empty cells and assign them appropriately
		int remainingNumberOfMines = numberOfMines;
		while (remainingNumberOfMines) { // if there exist any unassigned mine, assign it
			int loc = rand() % size;
			if (numberOfNeighboringMines[loc] == 0) { // it is not allocated any mine yet.
				numberOfNeighboringMines[loc] = MineCell;
				--remainingNumberOfMines;
			}
		} // let no assigned mines left

		// calculate neighboring Cells
		for (int cellNo = 0; cellNo < size; cellNo++) {
			int numberOfMines = 0;
			if (isMine(cellNo)) continue;
			auto neighbors = getNeighbors(cellNo);
			for (auto neighbor : neighbors) {
				if (!isMine(neighbor)) continue;
				++numberOfMines;
			}
			numberOfNeighboringMines[cellNo] = numberOfMines;
		}
	}

	vector<int> getNeighbors(int cellNo)
	{
		vector<int> neighbors;

		const int candidates[]{ cellNo - width - 1, cellNo - width, cellNo - width + 1,
							cellNo - 1, cellNo + 1,
							cellNo + width - 1, cellNo + width, cellNo + width + 1 };
		for (auto candidate : candidates) {
			if (!isNeighbor(candidate, cellNo)) continue;
			auto neighbor = candidate;
			neighbors.push_back(neighbor);
		}
		return neighbors;
	}

	int pos2CellNo(const Position& pos) const {
		return pos.x + pos.y * width;
	}

	Position cellNo2Pos(int cellNo) const {
		return Position{ cellNo % width, cellNo / width };
	}

	bool isMine(int cellNo) const { return numberOfNeighboringMines[cellNo] == MineCell; }

	bool isNeighbor(const Position& candidate, const Position& cellNo) const
	{
		if (validatePosition(candidate) == false) return false;
		if (cellNo.x == 0 && candidate.x == (width - 1)) return false;
		if (cellNo.x == (width - 1) && candidate.x == 0) return false;
		return true;
	}
		
	bool isNeighbor(int candidate, int cellNo) const {
		return isNeighbor(cellNo2Pos(candidate), cellNo2Pos(cellNo));
	}

	bool validatePosition(int cellNo) const { return cellNo >= 0 && cellNo < size; }
	
	bool validatePosition(const Position& pos) const {
		return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
	}

	void evaluateGameOver()
	{
		// find the number of marked cells
		int numberOfMarked = 0;
		for (int cellNo = 0; cellNo < size; cellNo++) {
			if (cellStates[cellNo] == CellState::Marked) {
				numberOfMarked++;
			}
		}
		// if the number of marked cells does not match with the number of mines
		// it will not end yet.
		if (numberOfMarked != numberOfTotalMines) return;

		// if the marked cells do not matches with the mines, it won't end yet.
		for (int cellNo = 0; cellNo < size; cellNo++) {
			if (cellStates[cellNo] == CellState::Marked && isMine(cellNo) == false )
				return;
		}

		// if matched, set the flag ready to end the game.
		for (int cellNo = 0; cellNo < size; cellNo++) {
			if (cellStates[cellNo] == CellState::Marked) {
				cellStates[cellNo] = CellState::Open;
				setShape(Mined, cellNo);
			}
		}
		Borland::gotoxy(local2screen({ width / 2 - (int)strlen("You Won.")/2, height * 2 + 2}));
		printf("You Won.");
		bGameOver = true;
	}

	void dig(const int cellNo)
	{
		if (validatePosition(cellNo) == false) return;
		if (cellStates[cellNo] == CellState::Open) return;

		// if you click the cell containing the mine, you will lose.
		if (isMine(cellNo)) {
			cellStates[cellNo] = CellState::Open;
			setShape(Mined, cellNo);
			Borland::gotoxy(local2screen({ width/2 - (int)strlen("You Lost.")/2, height * 2 + 2}));
			printf("You Lost.");
			bGameOver = true;
			return;
		}

		// open the cell
		cellStates[cellNo] = CellState::Open;
		setShape(numberOfNeighboringMines[cellNo] + '0', cellNo);

		// if there are no mine cell nearby, open all neighbors.
		if (numberOfNeighboringMines[cellNo] != 0) return;

		// open all neighboring cells if there are no neighbor containing the mine.
		auto neighbors = getNeighbors(cellNo);
		for (auto neighbor : neighbors) {
			dig(neighbor);
		}
	}

	void onMarking(const Position& pos)
	{
		if (validatePosition(pos) == false) return;
		int cellNo = pos2CellNo(pos);
		switch (cellStates[cellNo]) {
		case CellState::Marked:
			cellStates[cellNo] = CellState::Closed;
			setShape(Closed, cellNo);
			break;
		case CellState::Closed:
			cellStates[cellNo] = CellState::Marked;
			setShape(Marked, cellNo);
			break;
		}
	}

	void onClick(const Position& pos)
	{
		if (validatePosition(pos) == false) return;

		int cellNo = pos2CellNo(pos);

		switch (cellStates[cellNo]) {
		case CellState::Closed:
		case CellState::Marked:
			dig(cellNo);
			break;
		}
	}

	void onEvaluation(const Position& pos)
	{
		if (validatePosition(pos) == false) return;
		auto cellNo = pos2CellNo(pos);
		if (cellStates[cellNo] != CellState::Open) return;

		auto neighbors = getNeighbors(cellNo);

		int numberOfMarked = 0;
		for (auto neighbor : neighbors)
		{
			if (cellStates[neighbor] == CellState::Closed && isMine(neighbor)) return;
			if (cellStates[neighbor] == CellState::Marked) numberOfMarked++;
		}
		if (numberOfMarked != numberOfNeighboringMines[cellNo]) return;

		for (auto neighbor : neighbors)
		{
			if (cellStates[neighbor] == CellState::Marked) continue;
			dig(neighbor);
		}
	}
	
public:
	Map(int numberOfMines = 10, int width = 10, int height = 10, const Position& pos = { 0, 0 })
		: width(width), height(height), size(width*height), 
		numberOfTotalMines(numberOfMines), bGameOver(false),
		GameObject("", pos, {width, height}), 
		numberOfNeighboringMines{ new int[size] }, cellStates{ new CellState[size] }
	{
		initializeCells(numberOfMines);
	}

	~Map() {
		delete[] numberOfNeighboringMines;
		numberOfNeighboringMines = nullptr;
		delete[] cellStates;
		cellStates = nullptr;
	}
	


	void update() override 
	{	
		if (input->getMouseButtonDown(0)) {
			onClick(screen2local(input->getMousePosition()));
		}
		if (input->getMouseButtonDown(1)) {
			onMarking(screen2local(input->getMousePosition()));
		}
		if (input->getMouseButtonDown(2)) {
			onEvaluation(screen2local(input->getMousePosition()));
		}
		evaluateGameOver();
	}

	bool isGameOver() const {
		return bGameOver;
	}	
};


int main()
{	
	Screen* screen = Screen::GetInstance();
	Input*  input = Input::GetInstance();
	
	vector<Map*> objs;
	objs.push_back(new Map{ 10, 10, 10, { 0, 0 }  });
	Sleep(1000);
	objs.push_back(new Map{ 10, 10, 10, { 25, 0 } });
	
	// Get the standard inp
	
	while (1) {
		bool completed = true;
		for (auto obj : objs) {
			if (obj->isGameOver() == false) {
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

void Input::errorExit(const char* lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);
	
	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

bool Input::getKeyDown(WORD virtualKeyCode)
{
	// TODO: NOT FULLY IMPLEMENTED YET
	return getKey(virtualKeyCode); 
}

bool Input::getKey(WORD virtualKeyCode)
{
	if (cNumRead == 0) return false;

	for (int i = 0; i < cNumRead; i++)
	{
		if (irInBuf[i].EventType != KEY_EVENT) continue;

		if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == virtualKeyCode &&
			irInBuf[i].Event.KeyEvent.bKeyDown == TRUE ) {
			return true;
		}
	}
	return false;
}

bool Input::getKeyUp(WORD virtualKeyCode)
{
	if (cNumRead == 0) return false;

	for (int i = 0; i < cNumRead; i++)
	{
		if (irInBuf[i].EventType != KEY_EVENT) continue;

		if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == virtualKeyCode &&
			irInBuf[i].Event.KeyEvent.bKeyDown == FALSE) {
			return true;
		}
	}
	return false;
}

void Input::keyEventProc(KEY_EVENT_RECORD ker)
{
	Borland::gotoxy(0, 11);
	printf("%s\r", blankChars);
	if (ker.bKeyDown) {
		
	}
	switch (ker.wVirtualKeyCode) {
		
	case VK_LBUTTON:
		break;
	case VK_BACK:
		break;
	case VK_RETURN:
		break;
	case VK_LEFT:
		break;
	case VK_UP:
		break;
	default:
		if (ker.wVirtualKeyCode >= 0x30 && ker.wVirtualKeyCode <= 0x39)
			printf("Key event: %c %d", ker.wVirtualKeyCode - 0x30 + '0', ker.wRepeatCount);
		else printf("Key event: %c %d", ker.wVirtualKeyCode - 0x41 + 'A', ker.wRepeatCount);
		break;
	}
	
	Borland::gotoxy(0, 0);
}

bool Input::getMouseButtonDown(unsigned short number)
{
	for (int i = 0; i < cNumRead; i++) {
		if (irInBuf[i].EventType != MOUSE_EVENT) continue;

		auto& mouseEvent = irInBuf[i].Event.MouseEvent;
		if (mouseEvent.dwEventFlags != 0) continue;

		if ( (number == 0 && mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) ||
			 (number == 1 && mouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) ||
			 (number == 2 && 
				 (mouseEvent.dwButtonState == (FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED)
				  || mouseEvent.dwButtonState == FROM_LEFT_2ND_BUTTON_PRESSED) ) ) {
			mousePosition = mouseEvent.dwMousePosition;
			return true;
		}
	}
	return false;
}

void Input::mouseEventProc(MOUSE_EVENT_RECORD mer)
{
	Borland::gotoxy(0, 12);
	printf("%s\r", blankChars);
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	printf("Mouse event: ");
	
	switch (mer.dwEventFlags)
	{
	case 0:
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			printf("left button press %d %d\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			printf("right button press \n");
		}
		else
		{
			printf("button press %x\n", mer.dwButtonState);
		}
		break;
	case DOUBLE_CLICK:
		printf("double click\n");
		break;
	case MOUSE_HWHEELED:
		printf("horizontal mouse wheel\n");
		break;
	case MOUSE_MOVED:
		printf("mouse moved %d %d\n", mer.dwMousePosition.X, mer.dwMousePosition.Y);
		break;
	case MOUSE_WHEELED:
		printf("vertical mouse wheel\n");
		break;
	default:
		printf("unknown\n");
		break;
	}
	Borland::gotoxy(0, 0);
}

void Input::resizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	Borland::gotoxy(0, 13);
	printf("%s\r", blankChars);
	printf("Resize event: ");
	printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
	Borland::gotoxy(0, 0);
}