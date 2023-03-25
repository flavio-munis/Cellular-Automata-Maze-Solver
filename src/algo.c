#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "menu.h"
#include "board.h"
#include "moviments.h"
#include "error_handler.h"
#include "file_handler.h"
#include "algo.h"


// Creates a new instance of the Info type
Info* createInfo(Board* currentBoard, MovimentVec* moviments) {

	Info* newInfo = (Info*) malloc(sizeof(Info));

	checkNullPointer((void*) newInfo);

	newInfo -> currentBoard = currentBoard;
	newInfo -> moviments = moviments;

	return newInfo;
}

// Creates a copy from the current struct and returns it's pointer
Info* copyInfo(Info* currentInfo) {

	Info* newInfo;
	Board* newBoard = copyBoard(currentInfo -> currentBoard);
	MovimentVec* newMovimentVec = copyMovimentVec(currentInfo -> moviments);
		
	checkNullPointer((void*) newBoard);
	checkNullPointer((void*) newMovimentVec);

	newInfo = createInfo(newBoard, newMovimentVec);
	return newInfo;
}

// Frees all memory allocated to info struct
void freeInfo(Info* currentInfo) {
	freeBoard(currentInfo -> currentBoard);
	freeMovimentVec(currentInfo -> moviments);
	free(currentInfo);
}

// Calculates the player distance to the finish square
double distanceToFinish(Board* currentBoard) {

	double sizeCol = (double) currentBoard -> sizeCol - 1;
	double sizeRow = (double) currentBoard -> sizeRow - 1;
    double playerPositionX = (double) currentBoard -> player.coordinate[0];
	double playerPositionY = (double) currentBoard -> player.coordinate[1];

	return sqrt(pow((sizeCol - playerPositionX), 2.0) + pow((sizeRow - playerPositionY), 2.0));	
}

// Auxiliar function that discover if player can win in the next move
static inline bool gameWonNextMove(NextMoves* nextMovements) {
	return (nextMovements -> up == GAME_WON || nextMovements -> down == GAME_WON || nextMovements -> right == GAME_WON || nextMovements -> left == GAME_WON);
}

// Auxiliar functiona that returns if a player is gonna be traped in the next move
static inline bool gameOverNextMove(NextMoves* nextMovements) {
	return (nextMovements -> up == GAME_OVER && nextMovements -> down == GAME_OVER && nextMovements -> right == GAME_OVER && nextMovements -> left == GAME_OVER);
}

Info* closerToFinish(Info* currentInfo, Info* upInfo, Info* downInfo, Info* leftInfo, Info* rightInfo) {

	double currentDistance = distanceToFinish(currentInfo -> currentBoard);
	double distanceUp;
	double distanceDown;
	double distanceLeft;
	double distanceRight;
	double smallestDistance = currentDistance;
	Info* result = currentInfo;
	NextMoves* nextMovements;
	
	if(upInfo) {

		nextMovements = getNextMoviments(upInfo -> currentBoard);

		if(gameWonNextMove(nextMovements)){
			free(nextMovements);
			return copyInfo(upInfo);
		} else if(!gameOverNextMove(nextMovements)) {
			distanceUp = distanceToFinish(upInfo -> currentBoard);

			if(smallestDistance > distanceUp) {
				result = upInfo;
				smallestDistance = distanceUp;
			}
		}

		free(nextMovements);
}
		
	if(downInfo) {

		nextMovements = getNextMoviments(downInfo -> currentBoard);
		
		if(gameWonNextMove(nextMovements))
			return copyInfo(downInfo);
		else if(!gameOverNextMove(nextMovements)){
			distanceDown = distanceToFinish(downInfo -> currentBoard);

			if(smallestDistance > distanceDown) {
				result = downInfo;
				smallestDistance = distanceDown;
			}
		}

		free(nextMovements);
	}

	if(leftInfo) {

		nextMovements = getNextMoviments(leftInfo -> currentBoard);

		if(gameWonNextMove(nextMovements))
			return copyInfo(leftInfo);
		else if(!gameOverNextMove(nextMovements)){
			distanceLeft = distanceToFinish(leftInfo -> currentBoard);

			if(smallestDistance > distanceLeft) {
				result = leftInfo;
				smallestDistance = distanceLeft;
			}
		}

		free(nextMovements);
	}

	if(rightInfo) {

		nextMovements = getNextMoviments(rightInfo -> currentBoard);

		if(gameWonNextMove(nextMovements))
			return copyInfo(rightInfo);
		else if(!gameOverNextMove(nextMovements)){
			distanceRight = distanceToFinish(rightInfo -> currentBoard);

			if(smallestDistance > distanceRight) {
				result = rightInfo;
				smallestDistance = distanceRight;
			}
		}

		free(nextMovements);
	}

	if(result == currentInfo) {
		if(upInfo && distanceUp)
			if(upInfo -> moviments -> totalElements > result -> moviments -> totalElements)
				result = upInfo;

		if(downInfo && distanceDown)
			if(downInfo -> moviments -> totalElements > result -> moviments -> totalElements)
				result = downInfo;

		if(leftInfo && distanceLeft)
			if(leftInfo -> moviments -> totalElements > result -> moviments -> totalElements)
				result = leftInfo;

		if(rightInfo && distanceRight)
			if(rightInfo -> moviments -> totalElements > result -> moviments -> totalElements)
				result = rightInfo;
	}
	
	return copyInfo(result);
}

void auxDiscoverOptimalPath(Info* currentInfo, enum MovimentStates currentMove) {

	int* playerCoordinates = playerNewPosition(currentInfo -> currentBoard, currentMove);

	addToMovimentVec(currentInfo -> moviments, currentMove);
	updateBoard(currentInfo -> currentBoard, playerCoordinates[0], playerCoordinates[1]);
	free(playerCoordinates);
}

// Recursive function that searches through n possibles moviments, beign n the depth of search, and returns the one which is termitates closer to the finish square, don't end up in a dead-end, and have more moves.
Info* discoverOptimalPath(Info* currentInfo, int depth) {
	
	if(depth == 0){
		return copyInfo(currentInfo);
	} else {
		Info* newInfo = NULL;
		Info* upInfo = NULL, *downInfo = NULL, *rightInfo = NULL, *leftInfo = NULL;
		Info* result = NULL;
		NextMoves* nextUpdate = getNextMoviments(currentInfo -> currentBoard);
		
		if(playerHasWon(currentInfo -> currentBoard)) {
			free(nextUpdate);
			return copyInfo(currentInfo);
		}
			
		if(nextUpdate -> up != IMPOSSIBLE && nextUpdate -> up != GAME_OVER) {
			newInfo = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfo, UP);
			
			upInfo = discoverOptimalPath(newInfo, depth - 1);

			freeInfo(newInfo);
		} else if(nextUpdate -> up == GAME_WON) {
			newInfo = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfo, UP);

			free(nextUpdate);
			
			return newInfo;
		}
		
		if(nextUpdate -> down != IMPOSSIBLE && nextUpdate -> down != GAME_OVER) {
			newInfo = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfo, DOWN);			
			
			downInfo = discoverOptimalPath(newInfo, depth - 1);
			freeInfo(newInfo);
		} else if(nextUpdate -> down == GAME_WON) {
			newInfo = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfo, DOWN);

			free(nextUpdate);

			if(upInfo)
				freeInfo(upInfo);
			
			return newInfo;
		}

		if(nextUpdate -> left != IMPOSSIBLE && nextUpdate -> left != GAME_OVER) {
			newInfo = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfo, LEFT);

			leftInfo = discoverOptimalPath(newInfo, depth - 1);
			freeInfo(newInfo);
		} else if(nextUpdate -> left == GAME_WON) {
			newInfo = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfo, LEFT);

			free(nextUpdate);
			
			if(upInfo)
				freeInfo(upInfo);
			
			if(downInfo)
				freeInfo(downInfo);
			
			return newInfo;
		}
		
		if(nextUpdate -> right != IMPOSSIBLE && nextUpdate -> right != GAME_OVER) {
			newInfo = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfo, RIGHT);

			rightInfo = discoverOptimalPath(newInfo, depth - 1);
			freeInfo(newInfo);
		} else if(nextUpdate -> up == GAME_WON) {
			newInfo = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfo, RIGHT);

			free(nextUpdate);

			if(upInfo)
				freeInfo(upInfo);
			
			if(downInfo)
				freeInfo(downInfo);

			if(leftInfo)
				freeInfo(leftInfo);
			
			return newInfo;
		}
		
		free(nextUpdate);

		result = closerToFinish(currentInfo, upInfo, downInfo, leftInfo, rightInfo);
			
		if(upInfo)
			freeInfo(upInfo);

		if(downInfo)
			freeInfo(downInfo);

		if(leftInfo)
			freeInfo(leftInfo);

		if(rightInfo)
			freeInfo(rightInfo);
		
		return result;
	}
}

// Autoplay through the entire maze and writes the winning path to a file
void autoPlay(Board* currentBoard, int depth) {

	MovimentVec* moviments = initMovimentVec();
	Info* currentInfo = createInfo(currentBoard, moviments);
	size_t* currentIndex = &moviments -> totalElements;
	Info* result;
	int* playerCoordinates;
	enum MovimentType newMoviment;
	bool gameWonState = false;

	puts("Algorithm is Searching for a Path...\n");
	
	while(!gameOver(currentInfo -> currentBoard) && !playerHasWon(currentInfo -> currentBoard)) {
		
		if(!gameWonState) {
			result = discoverOptimalPath(currentInfo, depth);

			if(result -> moviments -> totalElements <= currentInfo -> moviments -> totalElements)
				deadEndPath();

			gameWonState = playerHasWon(result -> currentBoard);

			newMoviment = result -> moviments -> moves[*currentIndex];

			printf("\nMoviments: ");
			printMoviments(result -> moviments);
			printf("Total Elements:%ld\n", result -> moviments -> totalElements);
		
			playerCoordinates = playerNewPosition(currentInfo -> currentBoard, newMoviment);
			addToMovimentVec(currentInfo -> moviments, newMoviment);
			updateBoard(currentInfo -> currentBoard, playerCoordinates[0], playerCoordinates[1]);

			free(playerCoordinates);
			if(!gameWonState)
				freeInfo(result);
		} else {
			newMoviment = result -> moviments -> moves[*currentIndex];

			printf("\nMoviments: ");
			printMoviments(result -> moviments);
			printf("Total Elements:%ld\n", result -> moviments -> totalElements);
		
			playerCoordinates = playerNewPosition(currentInfo -> currentBoard, newMoviment);
			addToMovimentVec(currentInfo -> moviments, newMoviment);
			updateBoard(currentInfo -> currentBoard, playerCoordinates[0], playerCoordinates[1]);

			free(playerCoordinates);
			
		}
					 
		//printBoard(currentInfo -> currentBoard);
		printf("\nMoviments: ");
		printMoviments(currentInfo -> moviments);
		printf("Total Elements:%ld\n", currentInfo -> moviments -> totalElements);
		printf("\nCurrent Distance to Finish: %.2f\n\n",distanceToFinish(currentInfo -> currentBoard));
		printNextMoviments(currentInfo -> currentBoard);
		//puts("");
	}
	
	if(gameOver(currentInfo -> currentBoard))
		printf("The Algorithm Lost The Game! :(\n");
	else if(playerHasWon(currentInfo -> currentBoard)) {
		printf("The Algorithm Has Found a Correct Path! :)\n");
		saveBoardInfo(currentInfo -> moviments);
	} else {
		printf("The Algorithm Couldn't Finish The Game! :(\n");
	}

	freeInfo(currentInfo);
}
