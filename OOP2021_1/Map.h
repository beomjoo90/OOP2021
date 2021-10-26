#pragma once
#include <iostream>
#include "GameObject.h"
#include "Board.h"

class Map :
    public GameObject
{
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

	int* numberOfNeighboringMines;
	CellState* cellStates;

	int			numberOfTotalMines;
	bool		bGameOver;

	Board*		board;

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
		return pos.x >= 0 && pos.x < width&& pos.y >= 0 && pos.y < height;
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
			if (cellStates[cellNo] == CellState::Marked && isMine(cellNo) == false)
				return;
		}

		// if matched, set the flag ready to end the game.
		for (int cellNo = 0; cellNo < size; cellNo++) {
			if (cellStates[cellNo] == CellState::Marked) {
				cellStates[cellNo] = CellState::Open;
				setShape(Mined, cellNo);
			}
		}
		if (board) board->log("You Won.");

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
			if (board) {
				board->log("You Lost");
			}
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
		: width(width), height(height), size(width* height),
		numberOfTotalMines(numberOfMines), bGameOver(false),
		GameObject("", pos, { width, height }),
		numberOfNeighboringMines{ new int[size] }, cellStates{ new CellState[size] },
		board(new Board({ pos.x, pos.y + height }, { width, 0 }))
	{
		initializeCells(numberOfMines);
		children.push_back(board);
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
		GameObject::update();
	}

	bool isGameOver() const {
		return bGameOver;
	}
};

