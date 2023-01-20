# Minesweeper
Minesweeper game: console version written in C++

This project uses the following libraries: 
*iostream, cstdlib, ctime*

There are 4 levels of difficulty:
- Easy: board has size *7x7* and *7* mines
- Medium: board has size *10x10* and *20* mines
- Hard: board has size *15x15* and *45* mines
- Custom: player can select the board size as following: 7 < N < 21 (rows = columns = N) and the number of mines: N < mines < 3*N (*N* is board size, *mines* is mines count)

To play this game, you should enter a comand to show what move you intent to do: 
- To open a cell, you should enter *1*. 
- To mark a cell, you should enter *2*.
- To unmark a cell, you should enter *3*. 

After entering the command, you should type the coordinates of the cell you want to open / mark / unmark. The first column shows the number of each row, the first row shows the number of each column. 
You must only enter **numbers** of type int for coordinates. If you hit an empty cell, all cells next to it will be opened too.

To win this game, you should do one of the following:
- Find and mark all mines
- Open all cells which aren't mines

You will lose if you reveal a mine. The positions of all hidden mines are revealed after revealing a mine.

After winning/losing the game, you can choose wether to play again or quit with Y/N answer ( **Y** or **y** for *yes*, **N** or **n** for *no*).