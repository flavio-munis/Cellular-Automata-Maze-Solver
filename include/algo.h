#ifndef ALGO_HEADER_FILE
#define ALGO_HEADER_FILE

#include "board.h"
#include "moviments.h"

typedef struct info {
	MovimentVec* moviments;
	Board* currentBoard;
} Info;

void autoPlay(Board*, int);

#endif
