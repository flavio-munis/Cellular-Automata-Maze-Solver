#ifndef BOARD_HEADER_FILE
#define BOARD_HEADER_FILE

#include <stdbool.h>
#include "pieces.h"

typedef struct player {
	int8_t coordinate[2];
} Player;

typedef struct board {
	int8_t sizeCol;
	int8_t sizeRow;
	Piece** pieces;
	Player player;
} Board;

void debugBoard(Board*);
Board* createCustomBoard(char*,int,int);
void updateBoard(Board*, int, int);
bool gameOver(Board*);
bool playerHasWon(Board*);
int checkNeighbours(Board*, int, int);
void freeBoard(Board*);
Board* copyBoard(Board*);

#endif
