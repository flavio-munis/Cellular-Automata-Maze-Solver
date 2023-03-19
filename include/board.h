#ifndef BOARD_HEADER_FILE
#define BOARD_HEADER_FILE

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

void printBoard(Board*);
void debugBoard(Board*);
Board* createCustomBoard(char*,int,int);
void updateBoard(Board*);

#endif
