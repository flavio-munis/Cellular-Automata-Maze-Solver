#ifndef MOVEMENTS_HEADER_FILE
#define MOVEMENTS_HEADER_FILE

#include <stdbool.h>
#include "board.h"

enum MovimentStates {
	IMPOSSIBLE,
	OKAY,
	GAME_OVER,
	GAME_WON,
	START_PIECE
};

enum MovimentType {
	UP = 'U',
	DOWN = 'D',
	RIGHT = 'R',
	LEFT = 'L'
};

typedef struct movimentVec {
	enum MovimentType* moves;
	size_t movesSize;
	size_t totalElements;
} MovimentVec;

typedef struct nextMoves {
	enum MovimentStates up;
	enum MovimentStates down;
	enum MovimentStates right;
	enum MovimentStates left;
} NextMoves;

NextMoves* getNextMoviments(Board*);
bool isValidMove(Board*, char);
enum MovimentType convertCharToMove(char);
MovimentVec* initMovimentVec();
void addToMovimentVec(MovimentVec*, enum MovimentType);
void freeMovimentVec(MovimentVec*);
MovimentVec* copyMovimentVec(MovimentVec*);
bool compareMovimentVecs(MovimentVec*, MovimentVec*);
int* playerNewPosition(Board*, enum MovimentType);
void printMoviments(MovimentVec*);
void solveBoard(Board*, MovimentVec*);

#endif
