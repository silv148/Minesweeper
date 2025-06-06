﻿/**
*
* Solution to course project # 7
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2022/2023
*
* @author Silvia Zemedelska
* @idnumber 6MI0600237
* @compiler VC
*
* <file with all functions>
*
* link to github: https://github.com/silv148/Minesweeper
*
*/

#include <iostream>
#include <cstdlib>
#include <ctime>

const char MARKED_CELL = 'X', MINE = '@', DISP_CELL = '*', EMPTY_CELL = '0';

void PrintStart() {
	system("title = MINESWEEPER");
	std::cout << "Select level:" << std::endl <<
		"1 - Easy: Board size is 7x7, mines count is 7" << std::endl <<
		"2 - Medium: Board size is 10x10, mines count is 20" << std::endl <<
		"3 - Hard: Board size is 15x15, mines count is 45" << std::endl <<
		"4 - Custom: You can select the board size and the count of mines" << std::endl;
}

void ValidateInput(int& input, const int minVal, const int maxVal) {
	bool err = false;
	std::cin >> input;
	while (input <= minVal || input >= maxVal) {
		std::cout << "Enter number in range: [" <<
			minVal << " < N < " << maxVal << "]: " << std::endl;
		err = ((bool)(std::cin));
		if (!err) {
			std::cin.clear();
			std::cin.ignore();
		}
		std::cin >> input;
	}
}

void InitBoard(char** realBoard, const int size, const char symbol) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			realBoard[i][j] = symbol;
		}
	}
}

void PrintBoard(char** board, const int size) {
	std::cout << std::endl << "    ";
	for (int i = 1; i <= size; i++) {
		i < 10 ? std::cout << "   " << i : std::cout << "  " << i;
	}
	std::cout << std::endl << "   ";
	for (int i = 0; i <= (size * 2); i++) {
		std::cout << "--";
	}
	std::cout << "-" << std::endl;
	for (int i = 1; i <= size; i++) {
		i < 10 ? std::cout << "   " << i << " | " : std::cout << "  " << i << " | ";
		for (int j = 0; j < size; j++) {
			std::cout << board[i - 1][j] << " | ";
		}
		std::cout << std::endl;
	}
}

// increase the value of all cells next to mine with 1
void Hints(int x, int y, char** arr, const int size) {
	if (x < size && y < size) {
		if (x >= 0 && y >= 0) {
			if (arr[x][y] != MINE) {
				arr[x][y] += 1;
			}
		}
	}
}

bool IsMemberOfMinesArray(int* minesArray, int x, int y, const int& size) {
	for (int i = 0; i < size; i += 2)
		if (minesArray[i] == x && minesArray[i + 1] == y)
			return true;
	return false;
}

void InputMinesArray(int* minesArray, const int size) {
	for (int i = 0; i < size; i++) {
		minesArray[i] = -1;
	}
}

void MineGenerator(char** realBoard, const int size, int minesCount) {
	int minesArrSize = minesCount * 2;
	int* minesArray = new int[minesArrSize];
	InputMinesArray(minesArray, minesArrSize);
	for (int i = 0; i < minesArrSize; i += 2) {
		srand(time(NULL));
		int indexOne = rand() % size;
		int indexTwo = rand() % size;
		while (IsMemberOfMinesArray(minesArray, indexOne, indexTwo, minesArrSize)) {
			indexOne = rand() % size;
			indexTwo = rand() % size;
		}
		minesArray[i] = indexOne;
		minesArray[i + 1] = indexTwo;
		realBoard[indexOne][indexTwo] = MINE;
		for (int j = (indexOne - 1); j <= indexOne + 1; j++) {
			for (int k = indexTwo - 1; k <= indexTwo + 1; k++) {
				if (j == indexOne && k == indexTwo) {
					continue;
				}
				Hints(j, k, realBoard, size);
			}
		}
	}
	delete[] minesArray;
}

void DeleteMatrix(char** arr, const int size) {
	for (int i = 0; i < size; i++) {
		delete[] arr[i];
	}
	delete[] arr;
}

void RevealWhenEmpty(int x, int y, char** realBoard, char** dispBoard, int size) {
	bool isInBoundary = x >= 0 && y >= 0 && x < size && y < size;
	if (isInBoundary && realBoard[x][y] != MINE && dispBoard[x][y] == DISP_CELL) {
		dispBoard[x][y] = realBoard[x][y];
		if (realBoard[x][y] == EMPTY_CELL) {
			dispBoard[x][y] = ' ';
			for (int i = (x - 1); i <= x + 1; i++) {
				for (int j = y - 1; j <= y + 1; j++) {
					if (i == x && j == y) {
						continue;
					}
					RevealWhenEmpty(i, j, realBoard, dispBoard, size);
				}
			}
		}
	}
}

void RevealMine(char** dispBoard, char** realBoard, const int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (realBoard[i][j] == MINE) {
				dispBoard[i][j] = realBoard[i][j];
			}
		}
	}
	system("cls");
	PrintBoard(dispBoard, size);
}

void ReentereCoordinates(int& x, int& y, int maxVal) {
	std::cout << "Enter row number: ";
	std::cin >> x;
	std::cout << "Enter column number: ";
	std::cin >> y;
	x -= 1;
	y -= 1;
}

void OpenedCell(int& x, int& y, char** realBoard, char** dispBoard, const int size) {
	while (dispBoard[x][y] == realBoard[x][y] || dispBoard[x][y] == ' ') {
		std::cout << "This cell is already open! Try again!" << std::endl;
		ReentereCoordinates(x, y, size);
	}
}

void OutsideArrBoundary(int& x, int& y, const int size) {
	bool isInArrBoundary = (x >= 0 && y >= 0 && x < size&& y < size);
	while (!isInArrBoundary) {
		std::cout << "Coordinates out of boundary! Try again!" << std::endl;
		ReentereCoordinates(x, y, size);
		isInArrBoundary = (x >= 0 && y >= 0 && x < size&& y < size);
	}
}

void CellAlreadyMarked(int& x, int& y, char** dispBoard, const int size) {
	while (dispBoard[x][y] == MARKED_CELL) {
		std::cout << "This cell has been marked! Unmark it to open the cell!" << std::endl;
		ReentereCoordinates(x, y, size);
	}
}

void CellNotMarked(int& x, int& y, char** dispBoard, const int size) {
	while (dispBoard[x][y] != MARKED_CELL) {
		std::cout << "This cell is not marked! You can't unmark it!" << std::endl;
		ReentereCoordinates(x, y, size);
	}
}

void OpenCell(int x, int y, char** realBoard, char** dispBoard, const int size) {
	if (dispBoard[x][y] == MARKED_CELL) {
		CellAlreadyMarked(x, y, dispBoard, size);
	}
	switch (realBoard[x][y]) {
	case MINE: RevealMine(dispBoard, realBoard, size);
		std::cout << "You hit a mine :(" << std::endl;
		break;
	case EMPTY_CELL: RevealWhenEmpty(x, y, realBoard, dispBoard, size);
		break;
	default: dispBoard[x][y] = realBoard[x][y];
		break;
	}
}

void MarkCell(int x, int y, char** dispBoard, int size) {
	if (dispBoard[x][y] == MARKED_CELL) {
		CellAlreadyMarked(x, y, dispBoard, size);
	}
	dispBoard[x][y] = MARKED_CELL;
}

void UnmarkCell(int x, int y, char** dispBoard, int size) {
	if (dispBoard[x][y] != MARKED_CELL) {
		CellNotMarked(x, y, dispBoard, size);
	}
	dispBoard[x][y] = DISP_CELL;
}

void PrintInputInfo() {
	std::cout << "Enter 1 to open a cell." << std::endl <<
		"Enter 2 to mark a cell." << std::endl <<
		"Enter 3 to unmark a cell." << std::endl <<
		"YOUR COMMAND: ";
}

void ValidateCoordinates(int& x, int& y, char** realBoard, char** dispBoard, int size) {
	bool isInArrBoundary = (x >= 0 && y >= 0 && x < size&& y < size);
	if (!isInArrBoundary) {
		OutsideArrBoundary(x, y, size);
	}
	if (dispBoard[x][y] == realBoard[x][y] || dispBoard[x][y] == ' ') {
		OpenedCell(x, y, realBoard, dispBoard, size);
	}
}

void PlayerInputCommands(char** realBoard, char** dispBoard, int size) {
	char command = 0;
	PrintInputInfo();
	std::cin >> command;
	while (command < '1' || command > '3') {
		std::cout << "Invalid command! Enter again!" << std::endl << "YOUR COMMAND: ";
		std::cin >> command;
	}
	int x = 0, y = 0;
	std::cout << "Enter row number: ";
	std::cin >> x;
	x -= 1;
	std::cout << "Enter column number: ";
	std::cin >> y;
	y -= 1;
	ValidateCoordinates(x, y, realBoard, dispBoard, size);
	switch (command) {
	case '1':
		OpenCell(x, y, realBoard, dispBoard, size);
		break;
	case '2':
		MarkCell(x, y, dispBoard, size);
		break;
	case '3':
		UnmarkCell(x, y, dispBoard, size);
		break;
	default:
		std::cout << "Invalid command!" << std::endl;
		break;
	}
}

bool HasWon(char** realBoard, char** dispBoard, int size, int minesCount) {
	int minesCounter = 0;
	int unopenedCells = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (dispBoard[i][j] == MARKED_CELL && realBoard[i][j] == MINE) {
				minesCounter++;
			}
			if (dispBoard[i][j] == DISP_CELL) {
				unopenedCells++;
			}
		}
	}
	if (minesCounter - unopenedCells == minesCount) {
		return true;
	}
	if (minesCounter == minesCount) {
		return true;
	}
	return false;
}

bool HasLost(char** dispBoard, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (dispBoard[i][j] == MINE) {
				return true;
			}
		}
	}
	return false;
}

bool Replay() {
	char choice = ' ';
	std::cout << "Do you want to play again?" << std::endl;
	do {
		std::cout << "Enter your choice (Y/N): " << std::endl;
		std::cin >> choice;
		choice == 'y' ? choice = 'Y' : choice;
		choice == 'n' ? choice = 'N' : choice;
		switch (choice) {
		case 'Y':
			return true;
			break;
		case'N':
			std::cout << "Thank you for playing!" << std::endl;
			return false;
			break;
		}
		std::cout << "Invalid input!" << std::endl;
	} while (choice != 'Y' || choice != 'N');
}

void Play(char** realBoard, const int size, int minesCount) {
	char** displayboard = new char* [size];
	for (int i = 0; i < size; i++) {
		displayboard[i] = new char[size];
	}
	InitBoard(displayboard, size, DISP_CELL);
	MineGenerator(realBoard, size, minesCount);
	bool playersWin = HasWon(realBoard, displayboard, size, minesCount);
	do {
		system("cls");
		PrintBoard(displayboard, size);
		PlayerInputCommands(realBoard, displayboard, size);
		if (HasLost(displayboard, size)) {
			std::cout << "You can\'t make moves on this board anymore!" << std::endl;
			break;
		}
		playersWin = HasWon(realBoard, displayboard, size, minesCount);
	} while (!playersWin);
	if (playersWin) {
		std::cout << "CONGRATULATIONS!" << std::endl <<
			"You have won in this game!" << std::endl;
	}
	DeleteMatrix(displayboard, size);
}

void Create(int level) {
	const char MARKED_CELL = 'X', DISP_CELL = '*', EMPTY_CELL = '0';
	int boardSize = 0, minesCount = 0;
	switch (level) {
	case 1:
		boardSize = 7; minesCount = 7;
		break;
	case 2:
		boardSize = 10; minesCount = 20;
		break;
	case 3:
		boardSize = 15; minesCount = 45;
		break;
	case 4:
		system("cls");
		std::cout << "Enter size [7 < size < 21]: ";
		ValidateInput(boardSize, boardSize + 7, boardSize + 21);
		std::cout << "Enter mines count ["
			<< (boardSize - 1) << " < mines count < "
			<< (3 * boardSize + 1) << "]: ";
		ValidateInput(minesCount, boardSize - 1, 3 * boardSize + 1);
		break;
	}

	char** realBoard = new char* [boardSize];
	for (int i = 0; i < boardSize; i++) {
		realBoard[i] = new char[boardSize];
	}
	InitBoard(realBoard, boardSize, EMPTY_CELL);
	Play(realBoard, boardSize, minesCount);
	DeleteMatrix(realBoard, boardSize);
}

int main() {
	const int LEVEL_MIN_VALUE = 0, LEVEL_MAX_VALUE = 5;
	do {
		system("cls");
		int userInputBoardSize = 0, userInputLevel = 0;
		PrintStart();
		ValidateInput(userInputLevel, LEVEL_MIN_VALUE, LEVEL_MAX_VALUE);
		Create(userInputLevel);
	} while (Replay());
	exit(0);
}