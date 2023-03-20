#include <stdio.h>
#include <stdlib.h>
#include "error_handler.h"

// Read a board config from a file
int** readBoardFromFile(char* filename, int sizeRow, int sizeCol) {

	FILE* boardDescription = fopen(filename, "r");
	int** pseudoBoard;

	checkNullPointer((void*) boardDescription);
	
	// Mallocs all of pseudoBoard elements
	pseudoBoard = (int**) malloc(sizeRow * sizeof(int*));

	checkNullPointer((void*) pseudoBoard);
	
	for(int i = 0; i < sizeRow; i++) {
		pseudoBoard[i] = (int*) malloc(sizeCol * sizeof(int));
		checkNullPointer((void*) pseudoBoard[i]);
	}
	// Gets Board Config
	for(int i = 0; i < sizeRow; i++) {
		for(int j = 0; j < sizeCol; j++) {

			char temp = fgetc(boardDescription);

			pseudoBoard[i][j] = temp - '0';

			// Get rid of every other char
			fgetc(boardDescription);
		}
	}

	fclose(boardDescription);

	return pseudoBoard;
}
