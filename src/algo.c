#include <stdio.h>
#include <stdlib.h>
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

	double sizeCol = (double) currentBoard -> sizeCol;
	double sizeRow = (double) currentBoard -> sizeRow;
    double playerPositionX = (double) currentBoard -> player.coordinate[0];
	double playerPositionY = (double) currentBoard -> player.coordinate[1];

	return sqrt(pow((sizeCol - playerPositionX), 2.0) + pow((sizeRow - playerPositionY), 2.0));	
}

Info* closerToFinish(Info* currentInfo, Info* upInfo, Info* downInfo, Info* leftInfo, Info* rightInfo) {

	double currentDistance = distanceToFinish(currentInfo -> currentBoard);
	double distanceUp;
	double distanceDown;
	double distanceLeft;
	double distanceRight;
	double biggerDistance = currentDistance;
	Info* result = currentInfo;
	
	if(upInfo) {

		if(playerHasWon(upInfo -> currentBoard))
			return copyInfo(upInfo);
		
		distanceUp = distanceToFinish(upInfo -> currentBoard);
		result = (biggerDistance > distanceUp) ? upInfo : result;
	}
		
	if(downInfo) {

		if(playerHasWon(downInfo -> currentBoard))
			return copyInfo(downInfo);
		
		distanceDown = distanceToFinish(downInfo -> currentBoard);
		result = (biggerDistance > distanceDown) ? downInfo : result;
	}

	if(leftInfo) {

		if(playerHasWon(leftInfo -> currentBoard))
			return copyInfo(leftInfo);
		
		distanceLeft = distanceToFinish(leftInfo -> currentBoard);
		result = (biggerDistance > distanceLeft) ? leftInfo : result;
	}

	if(rightInfo) {

		if(playerHasWon(rightInfo -> currentBoard))
			return copyInfo(rightInfo);
		
		distanceRight = distanceToFinish(rightInfo -> currentBoard);
		result = (biggerDistance > distanceRight) ? rightInfo : result;
	}

	if(result == currentInfo) {

		if(upInfo)
			if(upInfo -> moviments -> totalElements > result -> moviments -> totalElements)
				result = upInfo;

		if(downInfo)
			if(downInfo -> moviments -> totalElements > result -> moviments -> totalElements)
				result = downInfo;

		if(leftInfo)
			if(leftInfo -> moviments -> totalElements > result -> moviments -> totalElements)
				result = leftInfo;

		if(rightInfo)
			if(rightInfo -> moviments -> totalElements > result -> moviments -> totalElements)
				result = rightInfo;
		
	}

	/*if(currentInfo)
		printf("Current Board Distance: %.2f\n", distanceToFinish(currentInfo -> currentBoard));

	if(upInfo)
		printf("Up Moviment Board Distance: %.2f\n", distanceToFinish(upInfo -> currentBoard));

	if(downInfo)
		printf("Down Moviment Board Distance: %.2f\n", distanceToFinish(downInfo -> currentBoard));

	if(leftInfo)
		printf("Left Moviment Board Distance: %.2f\n", distanceToFinish(leftInfo -> currentBoard));

	if(rightInfo)
	printf("Right Moviment Board Distance: %.2f\n", distanceToFinish(rightInfo -> currentBoard));

	puts("");*/
	
	return copyInfo(result);
}

Info* discoverOptimalPath(Info* currentInfo, int depth) {
	
	if(depth == 0){
		return copyInfo(currentInfo);
	} else {
		Info* newInfo = NULL;
		Info* upInfo = NULL, *downInfo = NULL, *rightInfo = NULL, *leftInfo = NULL;
		Info* result = NULL;
		NextMoves* nextUpdate = getNextMoviments(currentInfo -> currentBoard);
		int* playerCoordinates = NULL;

		if(nextUpdate -> up != IMPOSSIBLE && nextUpdate -> up != GAME_OVER) {
			newInfo = copyInfo(currentInfo);
			playerCoordinates = playerNewPosition(newInfo -> currentBoard, UP);

			addToMovimentVec(newInfo -> moviments, UP);
			
			updateBoard(newInfo -> currentBoard, playerCoordinates[0], playerCoordinates[1]);
			free(playerCoordinates);
			
			upInfo = discoverOptimalPath(newInfo, depth - 1);
			freeInfo(newInfo);
		}
		
		if(nextUpdate -> down != IMPOSSIBLE && nextUpdate -> down != GAME_OVER) {
			newInfo = copyInfo(currentInfo);
			playerCoordinates = playerNewPosition(newInfo -> currentBoard, DOWN);

			addToMovimentVec(newInfo -> moviments, DOWN);
			
			updateBoard(newInfo -> currentBoard, playerCoordinates[0], playerCoordinates[1]);
			free(playerCoordinates);
			
			downInfo = discoverOptimalPath(newInfo, depth - 1);
			freeInfo(newInfo);
		}

		if(nextUpdate -> left != IMPOSSIBLE && nextUpdate -> left != GAME_OVER) {
			newInfo = copyInfo(currentInfo);
			playerCoordinates = playerNewPosition(newInfo -> currentBoard, LEFT);

			addToMovimentVec(newInfo -> moviments, LEFT);

			updateBoard(newInfo -> currentBoard, playerCoordinates[0], playerCoordinates[1]);
			free(playerCoordinates);
			
			leftInfo = discoverOptimalPath(newInfo, depth - 1);
			freeInfo(newInfo);
		}
		
		if(nextUpdate -> right != IMPOSSIBLE && nextUpdate -> right != GAME_OVER) {
			newInfo = copyInfo(currentInfo);
			playerCoordinates = playerNewPosition(newInfo -> currentBoard, RIGHT);

			addToMovimentVec(newInfo -> moviments, RIGHT);
			
			updateBoard(newInfo -> currentBoard, playerCoordinates[0], playerCoordinates[1]);
			free(playerCoordinates);
			
			rightInfo = discoverOptimalPath(newInfo, depth - 1);
			freeInfo(newInfo);
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

void autoPlay(Board* currentBoard, int depth) {

	MovimentVec* moviments = initMovimentVec();
	Info* currentInfo = createInfo(currentBoard, moviments);
	Info* result;
	int* playerCoordinates;
	enum MovimentType newMoviment;
	
	while(!gameOver(currentInfo -> currentBoard) && !playerHasWon(currentInfo -> currentBoard)) {
		result = discoverOptimalPath(currentInfo, depth);
		newMoviment = result -> moviments -> moves[currentInfo -> moviments -> totalElements];

		printf("Moviments: ");
		printMoviments(result -> moviments);
		printf("Total Elements:%ld\n", result -> moviments -> totalElements);
	
		playerCoordinates = playerNewPosition(currentInfo -> currentBoard, newMoviment);
		addToMovimentVec(currentInfo -> moviments, newMoviment);
		updateBoard(currentInfo -> currentBoard, playerCoordinates[0], playerCoordinates[1]);

		free(playerCoordinates);
		freeInfo(result);
					 
		printBoard(currentInfo -> currentBoard);
		printf("Moviments: ");
		printMoviments(currentInfo -> moviments);
		printf("\nCurrent Distance to Finish: %.2f\n\n",distanceToFinish(currentInfo -> currentBoard));
	}
	
	if(gameOver(currentInfo -> currentBoard))
		printf("\nYou Lost :(\n");
	else if(playerHasWon(currentInfo -> currentBoard)) {
		printf("\nYou Won :)\n");
		saveBoardInfo(currentInfo -> moviments);
	} else {
		printf("Game is Not Finished!\n");
	}

	freeInfo(currentInfo);
}
