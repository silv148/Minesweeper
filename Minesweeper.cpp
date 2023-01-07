#include <iostream>
#include<cstdlib>
#include<ctime>

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

void Create(int level) {
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

	char** board = new char* [boardSize];

	for (int i = 0; i < boardSize; i++) {
		board[i] = new char[boardSize];
	}


	for (int i = 0; i < boardSize; i++) {
		delete[] board[i];
	}

}

int main()
{
	const int LEVEL_MIN_VALUE = 0, LEVEL_MAX_VALUE = 5;
	int userInputBoardSize = 0, userInputLevel = 0;
    PrintStart();
	ValidateInput(userInputLevel, LEVEL_MIN_VALUE, LEVEL_MAX_VALUE);
	Create(userInputLevel);
}