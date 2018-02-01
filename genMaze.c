#include "genMaze.h"

/*
* int get_num_walls(int numRows, int numColumns, cell *cells)
*
* DESCRIPTION
*
*   The get_num_walls() function returns the number of walls including
*   outside walls in a given maze (in the form of one-dimensional array
*   of cells of size numRows * numColumns).
*
* RETURN VALUE
*   The function get_num_walls() returns an integer representing
*   the number of walls of a given maze.
*/
int get_num_walls(int numRows, int numColumns, cell *cells)
{
	int row, column, numWalls = 0;

	cell(*cells2D)[8] = (cell(*)[8]) cells;

	for (row = 0; row < numRows; row++)
	{
		for (column = 0; column < numColumns; column++)
			if (cells2D[row][column].north == 1)
				numWalls++;

		for (column = 0; column < numColumns; column++)
			if (cells2D[row][column].west == 1)
				numWalls++;

		if (cells2D[row][numColumns - 1].east == 1)
			numWalls++;
	}

	for (column = 0; column < numColumns; column++)
		if (cells2D[numRows - 1][column].south == 1)
			numWalls++;

	return numWalls;
}

/*
* int print_maze(int numRows, int numColumns, cell *cells)
*
* DESCRIPTION
*
*   The print_maze() function prints a given maze (one-dimensional array
*   of size numRows * numColumns in text mode on the console screen. This
*   function also returns the number of walls including outside walls of
*   the given maze.
*
* RETURN VALUE
*   The function print_maze() also returns an integer representing
*   the number of walls of a given maze.
*/
int print_maze(int numRows, int numColumns, cell *cells)
{
	int row, column, numWalls = 0;

	cell(*cells2D)[8] = (cell(*)[8]) cells;

	for (row = 0; row < numRows; row++)
	{
		for (column = 0; column < numColumns; column++)
		{
			if (cells2D[row][column].north == 1)
			{
				printf("+--");
				numWalls++;
			}
			else
				printf("+  ");
		}

		printf("+\n");

		for (column = 0; column < numColumns; column++)
		{
			if (cells2D[row][column].west == 1)
			{
				printf("|  ");
				numWalls++;
			}
			else
				printf("   ");
		}

		if (cells2D[row][numColumns - 1].east == 1)
		{
			printf("|\n");
			numWalls++;
		}
		else
			printf("\n");
	}

	for (column = 0; column < numColumns; column++)
	{
		if (cells2D[numRows - 1][column].south == 1)
		{
			printf("+--");
			numWalls++;
		}
		else
			printf("+  ");
	}

	printf("+\n");

	return numWalls;
}

/*
* void gen_maze_rec(int startRow, int endRow, int startColumn, int endColumn,
*                   int numRows, int numColumns, cell *cells)
*
* DESCRIPTION
*   The gen_maze_rec() function is a recursive function which will be called
*   by the gen_maze() function to help generate a maze recursively.
*
* RETURN VALUE
*   None
*/
void gen_maze_rec(int startRow, int endRow, int startColumn, int endColumn, int numRows, int numColumns, cell *cells)
{
	int row, column;

	if (startRow + 1 == endRow || startColumn + 1 == endColumn)
		return;

	cell(*cells2D)[8] = (cell(*)[8]) cells;

	// Randomly pick a cell

	int pickRow = (rand() % (endRow - startRow - 1)) + startRow;
	int pickColumn = (rand() % (endColumn - startColumn - 1)) + startColumn;

	// Generate four long walls

	for (row = startRow; row < endRow; row++)
	{
		cells2D[row][pickColumn].east = 1;
		cells2D[row][pickColumn + 1].west = 1;
	}

	for (column = startColumn; column < endColumn; column++)
	{
		cells2D[pickRow][column].south = 1;
		cells2D[pickRow + 1][column].north = 1;
	}

	// Randomly pick a wall (not to remove)

	int pickWall = rand() % 4;

	// Ramdomly remove a wall of a cell from 3 out of 4 walls

	if (pickWall != 0)	// North
	{
		int rowToRemove = (rand() % (pickRow + 1 - startRow)) + startRow;
		cells2D[rowToRemove][pickColumn].east = 0;
		cells2D[rowToRemove][pickColumn + 1].west = 0;
	}

	if (pickWall != 1)	// West
	{
		int columnToRemove = (rand() % (pickColumn + 1 - startColumn)) + startColumn;
		cells2D[pickRow][columnToRemove].south = 0;
		cells2D[pickRow + 1][columnToRemove].north = 0;
	}

	if (pickWall != 2)	// South
	{
		int rowToRemove = (rand() % (endRow - pickRow - 1)) + pickRow + 1;
		cells2D[rowToRemove][pickColumn].east = 0;
		cells2D[rowToRemove][pickColumn + 1].west = 0;
	}

	if (pickWall != 3)	// East
	{
		int columnToRemove = (rand() % (endColumn - pickColumn - 1)) + pickColumn + 1;
		cells2D[pickRow][columnToRemove].south = 0;
		cells2D[pickRow + 1][columnToRemove].north = 0;
	}

	// Generate maze in each quarter

	gen_maze_rec(startRow, pickRow + 1, startColumn, pickColumn + 1, numRows, numColumns, cells);
	gen_maze_rec(startRow, pickRow + 1, pickColumn + 1, endColumn, numRows, numColumns, cells);
	gen_maze_rec(pickRow + 1, endRow, startColumn, pickColumn + 1, numRows, numColumns, cells);
	gen_maze_rec(pickRow + 1, endRow, pickColumn + 1, endColumn, numRows, numColumns, cells);
}

/*
* void gen_maze(int numRows, int numColumns, cell *cells)
*
* DESCRIPTION
*   The gen_maze() function generates a maze of size numRows by numColumns.
*   The result is put into a given two-dimensional array of cells. This
*   array of cells must by allocated before calling the gen_maze() function.
*   For example, to use this funciton to generate a maze of size 8 x 8,
*   use the following code segment:
*
*   cell *cells = (cell *) malloc (8 * 8 * sizeof(cell));
*   genMaze(8, 8, cells);
*
* RETURN VALUE
*   None.
*/
void gen_maze(int numRows, int numColumns, cell *cells)
{
	int row, column;

	cell(*cells2D)[8] = (cell(*)[8]) cells;

	// Set surrounding walls

	for (column = 0; column < numColumns; column++)
	{
		cells2D[0][column].north = 1;
		cells2D[numRows - 1][column].south = 1;
	}

	for (row = 1; row < numRows - 1; row++)
	{
		cells2D[row][0].west = 1;
		cells2D[row][numColumns - 1].east = 1;
	}

	cells2D[numRows - 1][0].west = 1;
	cells2D[0][numColumns - 1].east = 1;

	gen_maze_rec(0, numRows, 0, numColumns, numRows, numColumns, cells);
}
