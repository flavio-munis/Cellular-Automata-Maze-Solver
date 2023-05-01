#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "menu.h"
#include "moviments.h"
#include "newAlgo.h"
#include "board.h"
#include "error_handler.h"
#include "file_handler.h"
#include "cli_mode.h"

#define SIZE_ROW 7
#define SIZE_COL 8

// Commands parameters description definitions
char* customBoardParameters = "./executable_file custom-board [path_to_custom_board_file] [rows] [columns]";

char* autoPlayParameters = "./executable_file autoplay [depth] [optional: path_to_custom_board_file] [optional: rows] [optional: columns]";

char* checkSolutionParameters = "./executable_file check-solution [path_to_solution_file] [optional: path_to_custom_board_file] [optional: rows] [optional: columns]";

// Checks if a given string only has numbers
bool digitsOnly(char *s) {
    while (*s) {
        if (isdigit(*s++) == 0) return false;
    }
    return true;
}

// Show the list of commands valid in the program
void showListOfCommands() {

	puts("=== Welcome to the Program Solution for Stone Automata Challenge ===\n");
	puts("The List of Commands and It's Complete Description are the Following:");

	printf("\n*custom-board: This command is used for creating a new game from a custom board file. \nThe command description is the following: \n\n%s\n", customBoardParameters);

	printf("\n*autoplay: Finds a resolution path to the template board or the custom board passed along.\nThe command descriton is the following: \n\n%s\n", autoPlayParameters);

	printf("\n*check-solution: Checks if a solution file gets to a valid solution in a given board.\n\nThe command descriton is the following: \n\n%s\n", checkSolutionParameters);
	
}

// Executes the custom board command
void customBoard(int parametersSize, char* parameters[]) {

	char* pathToCustomBoardFile;
	char* sizeRow;
	char* sizeCol;
	Board* newBoard;
	
	if(parametersSize == 5) {

		pathToCustomBoardFile = parameters[2];
		sizeRow = parameters[3];
		sizeCol  = parameters[4];
		
		if(!digitsOnly(sizeRow) || !digitsOnly(sizeCol))
			invalidParametersError("custom-board", customBoardParameters);
		else {
			newBoard = createCustomBoard(pathToCustomBoardFile, atoi(sizeRow), atoi(sizeCol));
			playGame(newBoard);
		}
	} else
		invalidParametersError("custom-board", customBoardParameters);	
}

// Executes the autoplay command
void autoPlayCommand(int parametersSize, char** parameters, char* pathToBoard) {

	char* depth;
	Board* newBoard;

	if(parametersSize == 6) {
		
		char* sizeRow = parameters[4];
		char* sizeCol = parameters[5];  

		depth = parameters[2];
		
		if(!digitsOnly(sizeRow) || !digitsOnly(sizeCol) || !digitsOnly(depth))
			invalidParametersError("autoplay", autoPlayParameters);
		else{
			newBoard = createCustomBoard(pathToBoard, atoi(sizeRow), atoi(sizeCol));
			autoPlay(newBoard, atoi(depth));
		}
		
	} else if(parametersSize == 3) {
		
		depth = parameters[2];

		if(!digitsOnly(depth))
			invalidParametersError("autoplay", autoPlayParameters);
		else {
			newBoard = createCustomBoard(pathToBoard, SIZE_ROW, SIZE_COL);
			autoPlay(newBoard, atoi(depth));
		}
	  
	} else
		invalidParametersError("autoplay", autoPlayParameters);
	
}

// Executes the autosolve command
void checkSolution(int parametersSize, char** parameters, char* pathToBoard) {

	char* pathToSolutionFile;
	Board* newBoard;
	MovimentVec* moviments;

	if(parametersSize == 6) {

		char* sizeRow = parameters[4];
		char* sizeCol = parameters[5];
		
		pathToSolutionFile = parameters[2];

		if(!digitsOnly(sizeRow) || !digitsOnly(sizeCol))
			invalidParametersError("check-solution", checkSolutionParameters);
		else{
			newBoard = createCustomBoard(pathToBoard, atoi(sizeRow), atoi(sizeCol));
			moviments = readMovimentsFromFile(pathToSolutionFile);
			solveBoard(newBoard, moviments);

			freeBoard(newBoard);
			freeMovimentVec(moviments);
		}
		
	} else if(parametersSize == 3) {
		
		pathToSolutionFile = parameters[2];

		newBoard = createCustomBoard(pathToBoard, SIZE_ROW, SIZE_COL);
		moviments = readMovimentsFromFile(pathToSolutionFile);

		solveBoard(newBoard, moviments);

		freeBoard(newBoard);
		freeMovimentVec(moviments);
		
	} else
		invalidParametersError("check-solution", checkSolutionParameters);
	
}
