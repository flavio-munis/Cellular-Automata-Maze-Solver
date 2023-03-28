#ifndef ALGO_HEADER_FILE
#define ALGO_HEADER_FILE

#include "board.h"
#include "moviments.h"

typedef struct info {
	MovimentVec* moviments;
	Board* currentBoard;
} Info;

typedef struct paths {
	Info* currentInfo;
	float distToFinish;
	int8_t totalElements;
	struct paths* next;
} Paths;

void autoPlay(Board*, int);

#endif
