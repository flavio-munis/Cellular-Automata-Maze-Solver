#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "moviments.h"
#include "board.h"
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

// Creates a dynamic file name based on the current time
char* createFileName() {

	time_t currentTime;
	time(&currentTime);
	char* currentTimeString = ctime(&currentTime);
	char* fileName = calloc(32,sizeof(char));

	checkNullPointer((void*) fileName);

	// Removes new line characterer added in ctime function
	currentTimeString[strlen(currentTimeString) - 1] = '\0';

	// Concatenates file path
	strcat(fileName, "results/");
	strcat(fileName, currentTimeString);
	strcat(fileName, ".txt");
	
	return fileName;
}

// Saves winning movements list to a file
void saveBoardInfo(MovimentVec* moviments){

	char* fileName = createFileName();
	FILE* newFile = fopen(fileName, "w+");
	enum MovimentType* movimentArr = moviments -> moves;

	checkNullPointer((void*) newFile);
	
	for(int i = 0; i < moviments -> totalElements; i++) {
		if(i != moviments -> totalElements - 1)
			fprintf(newFile, "%c ", movimentArr[i]);
		else
			fprintf(newFile, "%c", movimentArr[i]);
	}
		
	free(fileName);
	fclose(newFile);
}
