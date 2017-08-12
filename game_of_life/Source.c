#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h> /* getch() and kbhit() */

#define TRUE 1
#define FALSE 0


#define ALIVE 254
#define DEAD 249

/* We need those params for keep know who alive and who dead
in the curr turn (for neighbors amount calculate)
and in the same time update them in the same loop*/
#define WILL_BE_ALIVE 2
#define WILL_BE_DEAD 3

#define TABLE_ROW 20
#define TABLE_COL 20

#define END_INPUT -1
#define DEMO_GAME -2
/* For neighbors amount calculate.*/
int offset[][2] = {
	{ -1, -1 },
	{ -1, 0 },
	{ -1, 1 },
	{ 0, -1 },
	{ 0, 1 },
	{ 1, -1 },
	{ 1, 0 },
	{ 1, 1 }
};

void init_table(int table[][TABLE_COL]);
void init_life_startingh_pos(int table[][TABLE_COL]);
void init_demo_game(int table[][TABLE_COL]);
void print_board(int table[][TABLE_COL]);
void game(int table[][TABLE_COL]);
int neighbors_amount(int table[][TABLE_COL], int i, int j);
int is_valid_cell(int row, int col);
int main()
{	


	int table[TABLE_ROW][TABLE_COL];
	int i;
	init_table(table);
	init_life_startingh_pos(table);
	for (i = 5; i > 0; --i)
	{
		system("CLS");
		printf("You can control the speed by pressing \'f\'- to fast and \'s\'- to slow.\nGame start in:\n   %d seconds\n", i);
		Sleep(1000);
	}
	game(table);
	system("PAUSE");
	return 0;
}

/*
the function "init_table" initiationg the  table with dead cells.
input:
table - the table of cells
output:
none
*/
void init_table(int table[][TABLE_COL])
{
	for (size_t i = 0; i < TABLE_ROW; i++)
	{
		for (size_t j = 0; j < TABLE_ROW; j++)
		{
			table[i][j] = DEAD;
		}
	}
}

/*
the function "init_life_startingh_pos" initiationg the the first generation.
input:
table - the table of cells
output:
none
*/
void init_life_startingh_pos(int table[][TABLE_COL])
{
	printf("Plesce enter the indexs of a liveing objects (in the format x,y ).\n");
	printf("Values from (0,0) up to (%d,%d), press %d to start the game.\n", TABLE_ROW - 1, TABLE_COL - 1, END_INPUT);
	printf("You can also start a DEMO game by pressing %d.\n",DEMO_GAME);
	char c = 0;
	int x = 0;
	int y = 0;
	do
	{
		scanf(" %d,%d", &x, &y);
		if (x == END_INPUT)
			continue;
		if (x == DEMO_GAME)
		{
			init_table(table); /* To make sure the table is empty*/

			init_demo_game(table);
			x = END_INPUT;
		}
		if (x >= 0 && y >= 0 && x < TABLE_ROW - 1 && y < TABLE_COL - 1)
		{
			table[x][y] = ALIVE;
			printf("object (%d,%d) successfully entered\n", x, y);
		}
		else
		{
			printf("object (%d,%d) cannot entered\nPlesce try again.", x, y);
		}
	} while (x != END_INPUT);

}


void init_demo_game(int table[][TABLE_COL])
{
	table[1+5][2+5] = ALIVE;
	table[3+5][1+5] = ALIVE;
	table[3+5][2+5] = ALIVE;
	table[2+5][4+5] = ALIVE;
	table[3+5][5+5] = ALIVE;
	table[3+5][6+5] = ALIVE;
	table[3+5][7+5] = ALIVE;
	return;
}

/*
the function "game" run the game.
Run the generations forever.
input:
table - the table of cells
output:
none
*/
void game(int table[][TABLE_COL])
{
	int game_run = TRUE;
	int neighbors = 0;
	int i, j;
	char c;
	int game_speed = 300;  /*Time to wait until new screen shows*/

	int temp_table[TABLE_ROW][TABLE_COL];
	while (game_run)
	{

		/*Check if a key was pressed.*/
			if (kbhit()) {
				c = getch();
				if (c == 'f' || c == 'F')
				{
					if (game_speed >= 100)
						game_speed -= 100;
				}
				else if (c == 's' || c == 'S')
				{
					game_speed +=100;
				}						
			}		

		system("CLS");
		print_board(table);
		Sleep(game_speed);
		for (i = 0; i < TABLE_ROW; i++)
		{
			for (j = 0; j < TABLE_COL; j++)
			{
				neighbors = neighbors_amount(table, i, j);
				if (table[i][j] == ALIVE && (neighbors > 3 || neighbors <= 1))
				{
					table[i][j] = WILL_BE_DEAD;  /* In this turn he still alive.*/
				}
				else if (table[i][j] == DEAD && neighbors == 3)
				{
					table[i][j] = WILL_BE_ALIVE; /* In this turn he still dead.*/
				}
			}
		}

	}
}

/*
the function "print_board" print the board.
input:
table - the table of cells
output:
none
*/
void print_board(int table[][TABLE_COL])
{
	int i, j;
	for (i = 0; i < TABLE_ROW; i++)
	{
		for (j = 0; j < TABLE_COL; j++)
		{

			/* Now every WILL_BE_ALIVE is alive and WILL_BE_DEAD is dead.*/

			if (table[i][j] == WILL_BE_ALIVE)
				table[i][j] = ALIVE;
			if (table[i][j] == WILL_BE_DEAD)
				table[i][j] = DEAD;
			printf("%c ", table[i][j]);
		}
		printf("\n");
	}
}

/*
the function "neighbors_amount" amount the neighbors of the given cell.
input:
table - the table of cells
i - the i position of the cell.
j - the j position of the cell.
output:
smount_of_neighbors - the smount of neighbors
*/
int neighbors_amount(int table[][TABLE_COL], int i, int j)
{
	int smount_of_neighbors = 0;
	int k = 0;
	int x = -1;
	int y = -1; /* Location of the new CELL, which
				we will find after adding offsets
				to the present value of i and j */

	for (k = 0; k < 8; k++)
	{
		x = i + offset[k][0];
		y = j + offset[k][1];
		if (is_valid_cell(x, y))
		{
			if (table[x][y] == ALIVE || table[x][y] == WILL_BE_DEAD) /* WILL_BE_DEAD means that he is alive at this generation. */
				++smount_of_neighbors;
		}
	}
	return smount_of_neighbors;
}

/*
the function "is_valid_cell" retrun TRUE if the position of the Cell in valid and FAlSE if not
input:
row - the row of the cell.
col - the col of the cell.
output:
TRUE / FALSE
*/
int is_valid_cell(int row, int col)
{
	if ((row >= 0 && row < TABLE_ROW) && (col >= 0 && col < TABLE_COL))
		return TRUE;
	return FALSE;
}