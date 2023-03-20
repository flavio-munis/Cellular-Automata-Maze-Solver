#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "menu.h"
#include "board.h"
#include "error_handler.h"
#include "cli_mode.h"

// Commands parameters description definitions
char* customBoardParameters = "./executable_file custom-board [path_to_custom_board_file] [rows] [columns]";


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

	printf("\n*custom-board: This command is used for creating a new game from a custom board file. \nThe command description is the follow: \n\n%s\n", customBoardParameters);
	
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
