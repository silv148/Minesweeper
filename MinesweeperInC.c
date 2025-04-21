#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MARKED_CELL 'X'
#define MINE '@'
#define DISP_CELL '*'
#define EMPTY_CELL '0'
#define MAX_CAPACITY 100


#define GREEN_COLOR "\x1b[32m"
#define RED_COLOR "\x1b[31m"
#define YELLOW_COLOR "\x1b[33m"
#define BLUE_COLOR "\x1b[94m"
#define RESET_COLOR "\x1b[0m"

void printStart();
void validateInput(int* input, const int minVal, const int maxVal);
void create(int level);
void initBoard(char board[MAX_CAPACITY][MAX_CAPACITY], const int size, const char symbol);
void play(char realBoard[MAX_CAPACITY][MAX_CAPACITY], const int size, int minesCount);
void mineGenerator(char realBoard[MAX_CAPACITY][MAX_CAPACITY], const int size, int minesCount);
void inputMinesArray(int* minesArray, const int size);
int isMemberOfMinesArray(int* minesArray, int x, int y, const int size);
void hints(int x, int y, char board[MAX_CAPACITY][MAX_CAPACITY], const int size);
int checkWin(char realBoard[MAX_CAPACITY][MAX_CAPACITY], char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size, int minesCount);
void playerInputCommands(char realBoard[MAX_CAPACITY][MAX_CAPACITY], char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size);
void printInputInfo();
void validateCoordinates(int* x, int* y, char realBoard[MAX_CAPACITY][MAX_CAPACITY], char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size, int* hasCommandChange);
void outsideArrBoundary(int* x, int* y, const int size);
int changeCommand();
void reenterCoordinates(int* x, int* y);
void revealedCell(int* x, int* y, char realBoard[MAX_CAPACITY][MAX_CAPACITY], char dispBoard[MAX_CAPACITY][MAX_CAPACITY], const int size, int* hasCommandChange);
void openCell(int x, int y, char realBoard[MAX_CAPACITY][MAX_CAPACITY], char dispBoard[MAX_CAPACITY][MAX_CAPACITY], const int size);
void cellAlreadyMarked(int* x, int* y, char dispBoard[MAX_CAPACITY][MAX_CAPACITY], const int size);
void revealMine(char dispBoard[MAX_CAPACITY][MAX_CAPACITY], char realBoard[MAX_CAPACITY][MAX_CAPACITY], const int size);
void printBoard(char board[MAX_CAPACITY][MAX_CAPACITY], const int size);
void revealWhenEmpty(int x, int y, char realBoard[MAX_CAPACITY][MAX_CAPACITY], char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size);
void markCell(int x, int y, char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size);
void unmarkCell(int x, int y, char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size);
void cellNotMarked(int* x, int* y, char dispBoard[MAX_CAPACITY][MAX_CAPACITY], const int size);
int checkDefeat(char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size);
int replay();
void deleteMatrix(char** arr, const int size);

void printStart() {
    system("title = MINESWEEPER");
    printf("Select level:\n");
    printf("1 - Easy: Board size is 7x7, mines count is 7\n");
    printf("2 - Medium: Board size is 10x10, mines count is 20\n");
    printf("3 - Hard: Board size is 15x15, mines count is 45\n");
    printf("4 - Custom: You can select the board size and the count of mines\n");
}

void validateInput(int* input, const int minVal, const int maxVal) {
    while (*input <= minVal || *input >= maxVal) {
        printf("Enter number in range: [%d < N < %d]:\n", minVal, maxVal);
        scanf_s("%d", input);
    }
}

void create(int level) {
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

        printf("Enter size [7 < size < 21]: ");
        validateInput(&boardSize, boardSize + 7, boardSize + 21);

        printf("Enter mines count [%d < mines count < %d]: ", (boardSize - 1), (3 * boardSize + 1));
        validateInput(&minesCount, boardSize - 1, 3 * boardSize + 1);
        break;
    }

    char realBoard[MAX_CAPACITY][MAX_CAPACITY];

    initBoard(realBoard, boardSize, EMPTY_CELL);
    play(realBoard, boardSize, minesCount);

}


void initBoard(char board[MAX_CAPACITY][MAX_CAPACITY], const int size, const char symbol) {
    for (int i = 0; i < size; i++) 
        for (int j = 0; j < size; j++)
            board[i][j] = symbol;
}

void play(char realBoard[MAX_CAPACITY][MAX_CAPACITY], const int size, int minesCount) {
    char displayBoard[MAX_CAPACITY][MAX_CAPACITY];

    initBoard(displayBoard, size, DISP_CELL);
    mineGenerator(realBoard, size, minesCount);
    int playersWin = checkWin(realBoard, displayBoard, size, minesCount);
    do {
        playerInputCommands(realBoard, displayBoard, size);

        if (checkDefeat(displayBoard, size)) {
            printf("\nYou can't make moves on this board anymore!\n\n");
            revealMine(displayBoard, realBoard, size);
            printf(BLUE_COLOR "\nYou have LOST this game :(\n\n" RESET_COLOR);
            break;
        }

        playersWin = checkWin(realBoard, displayBoard, size, minesCount);
    } while (!playersWin);

    if (playersWin)
        printf(YELLOW_COLOR "\nCONGRATULATIONS!\nYou have won in this game!\n\n" RESET_COLOR);
}

void mineGenerator(char realBoard[MAX_CAPACITY][MAX_CAPACITY], const int size, int minesCount) {
    int minesArrSize = minesCount * 2;
    int minesArray[MAX_CAPACITY*2];
    inputMinesArray(minesArray, minesArrSize);

    for (int i = 1; i < minesArrSize; i += 2) {
        srand(time(NULL));
        int indexOne = rand() % size;
        int indexTwo = rand() % size;

        while (isMemberOfMinesArray(minesArray, indexOne, indexTwo, minesArrSize)) {
            indexOne = rand() % size;
            indexTwo = rand() % size;
        }

        minesArray[i - 1] = indexOne;
        minesArray[i] = indexTwo;
        realBoard[indexOne][indexTwo] = MINE;

        for (int j = (indexOne - 1); j <= indexOne + 1; j++) {
            for (int k = indexTwo - 1; k <= indexTwo + 1; k++) {
                if (j == indexOne && k == indexTwo)
                    continue;

                hints(j, k, realBoard, size);
            }
        }
    }
}


void inputMinesArray(int* minesArray, const int size) {
    for (int i = 0; i < size; i++)
        minesArray[i] = -1;
}

int isMemberOfMinesArray(int* minesArray, int x, int y, const int size) {
    for (int i = 0; i < size; i += 2)
        if (minesArray[i] == x && minesArray[i + 1] == y)
            return 1;

    return 0;
}

void hints(int x, int y, char board[MAX_CAPACITY][MAX_CAPACITY], const int size) {
    if (x < size && y < size)
        if (x >= 0 && y >= 0)
            if (board[x][y] != MINE)
                board[x][y] += 1;
}

int checkWin(char realBoard[MAX_CAPACITY][MAX_CAPACITY], char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size, int minesCount) {
    int minesCounter = 0;
    int unrevealedCells = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (dispBoard[i][j] == MARKED_CELL && realBoard[i][j] == MINE)
                minesCounter++;

            if (dispBoard[i][j] == DISP_CELL)
                unrevealedCells++;
        }
    }

    if (minesCounter + unrevealedCells == minesCount)
        return 1;

    if (minesCounter == minesCount)
        return 1;

    return 0;
}

void playerInputCommands(char realBoard[MAX_CAPACITY][MAX_CAPACITY], char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size) {
    char command = 0;
    int hasCommandChange = 100;

    int x = 0, y = 0;

    do {
        system("cls");
        printBoard(dispBoard, size);
        hasCommandChange = 0;
        printInputInfo();
        scanf_s(" %c", &command);

        while (command < '1' || command > '3') {
            printf("Invalid command! Enter again!\nYOUR COMMAND: ");
            scanf_s(" %c", &command);
        }

        printf("Enter row number: ");
        scanf_s("%d", &x);
        x -= 1;
        printf("Enter column number: ");
        scanf_s("%d", &y);
        y -= 1;
        validateCoordinates(&x, &y, realBoard, dispBoard, size, &hasCommandChange);
    } while (hasCommandChange != 0);

    switch (command) {
    case '1':
        openCell(x, y, realBoard, dispBoard, size);
        break;
    case '2':
        markCell(x, y, dispBoard, size);
        break;
    case '3':
        unmarkCell(x, y, dispBoard, size);
        break;
    default:
        printf("Invalid command!\n");
        break;
    }
}

void printInputInfo() {
    printf("Enter 1 to open a cell.\n");
    printf("Enter 2 to mark a cell.\n");
    printf("Enter 3 to unmark a cell.\n");
    printf("YOUR COMMAND: ");
}

void validateCoordinates(int* x, int* y, char realBoard[MAX_CAPACITY][MAX_CAPACITY], char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size, int* hasCommandChange) {
    int isInArrBoundary = (*x >= 0 && *y >= 0 && *x < size&&* y < size);
    if (!isInArrBoundary)
        outsideArrBoundary(x, y, size);

    if (dispBoard[*x][*y] == realBoard[*x][*y] || dispBoard[*x][*y] == ' ')
        revealedCell(x, y, realBoard, dispBoard, size, hasCommandChange);
}

void outsideArrBoundary(int* x, int* y, const int size) {
    int isInArrBoundary = (*x >= 0 && *y >= 0 && *x < size&&* y < size);
    while (!isInArrBoundary) {
        printf("Coordinates out of boundary! Try again!\n");

        if (!changeCommand()) {
            reenterCoordinates(x, y);
            isInArrBoundary = (x >= 0 && y >= 0 && x < size&& y < size);
        }
        else return;
    }
}

int changeCommand() {
    char answer;
    do {
        printf("Do you want to change commands?(y/n): ");
        scanf_s(" %c", &answer);
        if (answer == 'n')
            return 0;
        else return 1;
    } while (answer != 'n' || answer != 'y');

    return 0;
}

void reenterCoordinates(int* x, int* y) {
    printf("Enter row number: ");
    scanf_s("%d", x);
    printf("Enter column number: ");
    scanf_s("%d", y);
    *x -= 1;
    *y -= 1;
}

void revealedCell(int* x, int* y, char realBoard[MAX_CAPACITY][MAX_CAPACITY], char dispBoard[MAX_CAPACITY][MAX_CAPACITY], const int size, int* hasCommandChange) {
    while (dispBoard[*x][*y] == realBoard[*x][*y] || dispBoard[*x][*y] == ' ') {
        printf("This cell is already open! Try again!\n");

        if (!changeCommand())
            reenterCoordinates(x, y);
        else {
            *hasCommandChange += 1;
            return;
        }
    }
}

void openCell(int x, int y, char realBoard[MAX_CAPACITY][MAX_CAPACITY], char dispBoard[MAX_CAPACITY][MAX_CAPACITY], const int size) {
    if (dispBoard[x][y] == MARKED_CELL)
        cellAlreadyMarked(&x, &y, dispBoard, size);

    switch (realBoard[x][y]) {
    case MINE:
        revealMine(dispBoard, realBoard, size);
        printf("You hit a mine :(\n");
        break;
    case EMPTY_CELL:
        revealWhenEmpty(x, y, realBoard, dispBoard, size);
        break;
    default:
        dispBoard[x][y] = realBoard[x][y];
        break;
    }
}

void cellAlreadyMarked(int* x, int* y, char dispBoard[MAX_CAPACITY][MAX_CAPACITY], const int size) {
    while (dispBoard[*x][*y] == MARKED_CELL) {
        printf("This cell has been marked! Unmark it to open the cell!\n");

        if (!changeCommand())
            reenterCoordinates(x, y);
        else return;
    }
}

void revealMine(char dispBoard[MAX_CAPACITY][MAX_CAPACITY], char realBoard[MAX_CAPACITY][MAX_CAPACITY], const int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (realBoard[i][j] == MINE)
                dispBoard[i][j] = realBoard[i][j];

    system("cls");
    printBoard(dispBoard, size);
}

void printBoard(char board[MAX_CAPACITY][MAX_CAPACITY], const int size) {
    printf("\n    ");
    for (int i = 1; i <= size; i++)
        i < 10 ? printf("   %d", i) : printf("  %d", i);

    printf("\n   ");
    for (int i = 0; i <= (size * 2); i++)
        printf("--");

    printf("-\n");
    for (int i = 1; i <= size; i++) {
        i < 10 ? printf("   %d | ", i) : printf("  %d | ", i);
        for (int j = 0; j < size; j++) {
            if (board[i - 1][j] == MARKED_CELL)
                printf(GREEN_COLOR "%c" RESET_COLOR " | ", board[i - 1][j]);
            else if (board[i - 1][j] == MINE) 
                printf(RED_COLOR "%c" RESET_COLOR " | ", board[i - 1][j]);
            else 
                printf("%c | ", board[i - 1][j]);
        }
        printf("\n\n");
    }
}

void revealWhenEmpty(int x, int y, char realBoard[MAX_CAPACITY][MAX_CAPACITY], char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size) {
    if (x < 0 || y < 0 || x >= size || y >= size)
        return;

    if (realBoard[x][y] != MINE && dispBoard[x][y] == DISP_CELL) {
        dispBoard[x][y] = realBoard[x][y];
        if (realBoard[x][y] == EMPTY_CELL) {
            dispBoard[x][y] = ' ';
            for (int i = (x - 1); i <= x + 1; i++) {
                for (int j = y - 1; j <= y + 1; j++) {
                    if (i == x && j == y)
                        continue;

                    revealWhenEmpty(i, j, realBoard, dispBoard, size);
                }
            }
        }
    }
}


void markCell(int x, int y, char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size) {
    if (dispBoard[x][y] == MARKED_CELL)
        cellAlreadyMarked(&x, &y, dispBoard, size);

    if (dispBoard[x][y] == DISP_CELL)
        dispBoard[x][y] = MARKED_CELL;
    else
        printf("You cannot mark this cell!\n");
}

void unmarkCell(int x, int y, char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size) {
    if (dispBoard[x][y] != MARKED_CELL)
        cellNotMarked(&x, &y, dispBoard, size);

    if(dispBoard[x][y] == MARKED_CELL)
        dispBoard[x][y] = DISP_CELL;
    else 
        printf("You cannot unmark this cell!\n");
}

void cellNotMarked(int* x, int* y, char dispBoard[MAX_CAPACITY][MAX_CAPACITY], const int size) {
    while (dispBoard[*x][*y] != MARKED_CELL) {
        printf("This cell is not marked! You can't unmark it!\n");

        if (!changeCommand())
            reenterCoordinates(x, y);
        else return;
    }
}

int checkDefeat(char dispBoard[MAX_CAPACITY][MAX_CAPACITY], int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (dispBoard[i][j] == MINE)
                return 1;

    return 0;
}

int replay() {
    char choice = ' ';
    printf("Do you want to play again?!\n");

    do {
        printf("Enter your choice (Y/N): \n");
        scanf_s(" %c", &choice);

        choice == 'y' ? choice = 'Y' : choice;
        choice == 'n' ? choice = 'N' : choice;

        switch (choice) {
        case 'Y':
            return 1;
            break;
        case'N':
            printf("Thank you for playing!\n");
            return 0;
            break;
        }
        printf("Invalid input!\n");
    } while (choice != 'Y' || choice != 'N');

    return 0;
}

int main() {
    const int LEVEL_MIN_VALUE = 0, LEVEL_MAX_VALUE = 5; 
    do {
        system("cls");
        int userInputLevel = 0;

        printStart();
        validateInput(&userInputLevel, LEVEL_MIN_VALUE, LEVEL_MAX_VALUE);
        create(userInputLevel);
    } while (replay());

    exit(0);
}
