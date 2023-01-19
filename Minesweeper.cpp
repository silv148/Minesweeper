#include <iostream>
#include<cstdlib>
#include<ctime>

const char MARKED_CELLS = 'X', MINE = '@', DISP_CELL = '*', EMPTY_CELL = '0';

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

void InitBoard(char** realboard,const int size, const char symbol)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			realboard[i][j] = symbol;
		}
	}
}

void PrintBoard(char** board, int size) 
{
	std::cout << std::endl << "    ";
	for (int i = 0; i < size; i++)
	{
		i < 10 ? std::cout << "   " <<i : std::cout << "  " << i;
	}
	std::cout << std::endl << "   ";
	for (int i = 0; i <= (size*2); i++)
	{
		std::cout << "--";
	}
	std::cout << std::endl;
	for (int i = 0; i < size; i++)
	{
		i < 10 ? std::cout << "   " << i << " | " : std::cout << "  " << i << " | ";
		for (int j = 0; j < size; j++)
		{
			std::cout << board[i][j] << " | ";
		}
		std::cout << std::endl;
	}
}



void Hints(int x, int y, char** board, const int size)
{
	if (x < size && y < size) {
		if (x >= 0 && y >= 0) {
			if (board[x][y] != MINE) {
				board[x][y] += 1;
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

void InputMinesArray(int* minesArray, int size) {
	for (int i = 0; i < size; i++) {
		minesArray[i] = -1;
	}
}

void MineGenerator(char** realBoard, int size, int minesCount)
{
	int minesArrSize = minesCount * 2;
	int* minesArray = new int[minesArrSize];
	InputMinesArray(minesArray, minesArrSize);
	for (int i = 0; i < minesArrSize; i += 2) {
		int indexOne = rand() % size;
		int indexTwo = rand() % size;
		while (IsMemberOfMinesArray(minesArray, indexOne, indexTwo, minesArrSize)) {
			srand(time(NULL));
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


void DeleteMatrix(char** arr, int size) {
	for (int i = 0; i < size; i++) {
		delete[] arr[i];
	}
	delete[] arr;
}

void RevealWhenEmpty(int x, int y, const char** realBoard, char** dispBoard, const int size)
{
	bool notMine = realBoard[x][y] != MINE;
	bool isDispCell = dispBoard[x][y] == DISP_CELL;
	bool isInArrBoundary = x >= 0 && y >= 0 && x < size && y < size;
	if (notMine && isDispCell && isInArrBoundary)
	{
		dispBoard[x][y] = realBoard[x][y];
		if (realBoard[x][y] == EMPTY_CELL)
		{
			dispBoard[x][y] = ' ';
			for (int i = (x - 1); i <= x + 1; i++) {
				for (int j = y - 1; j <= y + 1; j++) {
					if (i == x && j == y) {
						continue;
					}
					RevealWhenEmpty(i,j, realBoard, dispBoard, size);
				}
			}
		}
	}
}

void RevealMine(char** dispBoard, const char** realBoard, const int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (realBoard[i][j] == MINE)
			{
				dispBoard[i][j] = realBoard[i][j];
			}
		}
	}
	PrintBoard(dispBoard, size);
}

void Play(char** realBoard, int size, int minesCount) {
	char** displayboard = new char* [size];
	for (int i = 0; i < size; i++) {
		displayboard[i] = new char[size];
	}
	InitBoard(displayboard, size, DISP_CELL);
	MineGenerator(realBoard, size, minesCount);
	PrintBoard(displayboard, size);
	DeleteMatrix(displayboard, size);
}

void Create(int level) {
	const char MARKED_CELLS = 'X', DISP_CELL = '*', EMPTY_CELL = '0';
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

		std::cout << "Enter size [7 < size < 17]: ";
		ValidateInput(boardSize, 7, 17);

		std::cout << "Enter mines count ["
			<< (boardSize - 2) << " < mines count < "
			<< (3 * boardSize + 1) << "]: ";
		ValidateInput(minesCount, boardSize - 2, 3*boardSize - 1);
		break;
	}

	char** realboard = new char* [boardSize];
	for (int i = 0; i < boardSize; i++) {
		realboard[i] = new char[boardSize];
	}
	InitBoard(realboard, boardSize, EMPTY_CELL);
	Play(realboard, boardSize, minesCount);

	DeleteMatrix(realboard, boardSize);
}

int main()
{
	const int LEVEL_MIN_VALUE = 0, LEVEL_MAX_VALUE = 5;
	int userInputBoardSize = 0, userInputLevel = 0;
    PrintStart();
	ValidateInput(userInputLevel, LEVEL_MIN_VALUE, LEVEL_MAX_VALUE);
	Create(userInputLevel);
}