#ifndef BOARD_HEADER_FILE
#define BOARD_HEADER_FILE

#include <stdbool.h>
#include "pieces.h"

typedef struct player {
	int coordinate[2];
} Player;

typedef struct board {
	int sizeCol;
	int sizeRow;
	Piece** pieces;
	Player player;
} Board;

void debugBoard(Board*);
Board* createCustomBoard(char*,int,int);
void updateBoard(Board*, int, int);
bool gameOver(Board*);
bool playerHasWon(Board*);
int* playerNewPosition(Board*, char);
bool isValidMove(Board*, char);
int checkNeighbours(Board*, int, int);

#endif
