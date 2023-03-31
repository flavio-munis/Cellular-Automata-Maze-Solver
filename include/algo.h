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
	float score;
	int totalElements;
	struct paths* next;
} Paths;

typedef struct pathsTable {
	float score;
	MovimentVec* moviments;
	struct pathsTable* next;
} PathsTable;

void autoPlay(Board*, int);

#endif
