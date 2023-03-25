#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "board.h"
#include "pieces.h"
#include "error_handler.h"
#include "moviments.h"

// Checks if it's a valid move
bool isValidMove(Board* currentBoard, char direction) {

	int playerPositionX = currentBoard -> player.coordinate[0];
	int playerPositionY = currentBoard -> player.coordinate[1];
	int sizeRow = currentBoard -> sizeRow;
	int sizeCol = currentBoard -> sizeCol;

	switch(direction) {
	    case 'w':
			return playerPositionY - 1 >= 0;
		case 'a':
			return playerPositionX - 1 >= 0;
		case 's':
			return playerPositionY + 1 < sizeRow;
		case 'd':
			return playerPositionX + 1 < sizeCol;
	}

	return false;
}

// Convert Keyboard Input to moviment enum
enum MovimentType convertCharToMove(char input) {
	switch(input) {
	    case 'w':
			return UP;
		case 'a':
			return LEFT;
	    case 's':
			return DOWN;
		case 'd':
			return RIGHT;
	}

	return UP;
}

// Returns player new position on the board
int* playerNewPosition(Board* currentBoard, enum MovimentType direction) {

	int playerPositionX = currentBoard -> player.coordinate[0];
	int playerPositionY = currentBoard -> player.coordinate[1];
	int sizeRow = currentBoard -> sizeRow;
	int sizeCol = currentBoard -> sizeCol;
	int* newPosition = malloc(2*sizeof(int));

	checkNullPointer((void*) newPosition);
	
	newPosition[0] = playerPositionX;
	newPosition[1] = playerPositionY;
	
	switch(direction) {

	    case UP:
			if(playerPositionY - 1 >= 0)
				newPosition[1] = playerPositionY - 1;
			break;

		case LEFT:
			if(playerPositionX - 1 >= 0)
				newPosition[0] = playerPositionX - 1;
			break;

		case DOWN:
			if(playerPositionY + 1 < sizeRow)
				newPosition[1] = playerPositionY + 1;
			break;

		case RIGHT:
			if(playerPositionX + 1 < sizeCol)
				newPosition[0] = playerPositionX + 1;
			break;
	}

	return newPosition;
	
}

// Creates a new moviment vector
MovimentVec* initMovimentVec() {

	MovimentVec* newMovimentVec = (MovimentVec*) malloc(sizeof(MovimentVec));

	checkNullPointer((void*) newMovimentVec);
	
	newMovimentVec -> movesSize = 8;
	newMovimentVec -> totalElements = 0;
	newMovimentVec -> moves = calloc(newMovimentVec -> movesSize, sizeof(enum MovimentType));

	checkNullPointer((void*) newMovimentVec -> moves);

	return newMovimentVec;
}

// Reallocs size of vector
void reallocMovimentVec(MovimentVec* moviments) {
	moviments -> moves = realloc(moviments -> moves, moviments -> movesSize * sizeof(enum MovimentType));
}

// Adds a new element to vector
void addToMovimentVec(MovimentVec* moviments, enum MovimentType movimentMade) {
	
	if(moviments -> totalElements == moviments -> movesSize) {
		moviments -> movesSize *= 2;
		reallocMovimentVec(moviments);
		checkNullPointer((void*) moviments -> moves);
	}

	moviments -> moves[moviments -> totalElements] = movimentMade;
	moviments -> totalElements += 1;
	
}

// Frees all memory used by the moviment vector struct
void freeMovimentVec(MovimentVec* moviments) {
	free(moviments -> moves);
	free(moviments);
}

// Copies all of movimentsvec info to a new instance and returns a pointer to it
MovimentVec* copyMovimentVec(MovimentVec* moviments) {

	MovimentVec* newMoviments = initMovimentVec();

	checkNullPointer((void*) newMoviments);
	
	if(moviments -> movesSize > newMoviments -> movesSize) {
		newMoviments -> movesSize = moviments -> movesSize;
		reallocMovimentVec(newMoviments);
	}
	 
	newMoviments -> totalElements = moviments -> totalElements;
	
	for(int i = 0; i < newMoviments -> totalElements; i++)
		newMoviments -> moves[i] = moviments -> moves[i];

	return newMoviments;
}

// Print all elements presents in the moviments vector
void printMoviments(MovimentVec* moviments) {
	for(int i = 0; i < moviments -> totalElements; i++)
		printf("%c ", moviments -> moves[i]);

	puts("");
}

// Auxiliar function to print piece state
enum MovimentStates getNextMovimentsAux(enum PieceState pieceState, int neighboursAlive) {

	switch(pieceState) {

	    case START:
			return START_PIECE;

		case FINISH:
			return GAME_WON;

	    case DEAD:
			if(neighboursAlive == 2 || neighboursAlive == 3)
				return GAME_OVER;
			else
				return OKAY;

	    case ALIVE:
			if(neighboursAlive >= 4 && neighboursAlive < 7)
				return GAME_OVER;
			else
				return OKAY;
	}

	return IMPOSSIBLE;
}

// Prints if the possibles next movements results in endgame or nor
NextMoves* getNextMoviments(Board* currentBoard) {

	int playerPositionX = currentBoard -> player.coordinate[0];
	int playerPositionY = currentBoard -> player.coordinate[1];
	int sizeRow = currentBoard -> sizeRow;
	int sizeCol = currentBoard -> sizeCol;
	int neighboursAlive = 0;
	Piece** boardPieces = currentBoard -> pieces;
	enum PieceState currentPieceState;
	NextMoves* nextMoviments = malloc(sizeof(NextMoves));

	checkNullPointer((void*) nextMoviments);
	
	// Checks if up movement is valid
	if(playerPositionY != 0) {
		currentPieceState = boardPieces[playerPositionY - 1][playerPositionX].state;
		neighboursAlive = checkNeighbours(currentBoard, playerPositionY - 1, playerPositionX);
		
		nextMoviments -> up = getNextMovimentsAux(currentPieceState, neighboursAlive);
	} else
		nextMoviments -> up = IMPOSSIBLE;

	// Checks if down movement is valid
	if(playerPositionY != sizeRow - 1) {
		currentPieceState = boardPieces[playerPositionY + 1][playerPositionX].state;
		neighboursAlive = checkNeighbours(currentBoard, playerPositionY + 1, playerPositionX);

		nextMoviments -> down = getNextMovimentsAux(currentPieceState, neighboursAlive);
	} else
		nextMoviments -> down = IMPOSSIBLE;
	
	// Checks if left movement is valid
	if(playerPositionX != 0) {
		currentPieceState = boardPieces[playerPositionY][playerPositionX - 1].state;
		neighboursAlive = checkNeighbours(currentBoard, playerPositionY, playerPositionX - 1);

		nextMoviments -> left = getNextMovimentsAux(currentPieceState, neighboursAlive);
	} else
		nextMoviments -> left = IMPOSSIBLE;
	
	// Checks if right movement is valid
	if(playerPositionX != sizeCol - 1) {
		currentPieceState = boardPieces[playerPositionY][playerPositionX + 1].state;
		neighboursAlive = checkNeighbours(currentBoard, playerPositionY, playerPositionX + 1);

		nextMoviments -> right = getNextMovimentsAux(currentPieceState, neighboursAlive);
	} else
		nextMoviments -> right = IMPOSSIBLE;

	return nextMoviments;
}
