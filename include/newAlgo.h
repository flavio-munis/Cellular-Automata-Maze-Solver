#ifndef NEW_ALGO_HEADER_FILE
#define NEW_ALGO_HEADER_FILE

#include "board.h"
#include "moviments.h"

typedef struct pathsTable {
	float score;
	MovimentVec* moviments;
	struct pathsTable* next;
} PathsTable;

void autoPlay(Board*, int);

#endif
