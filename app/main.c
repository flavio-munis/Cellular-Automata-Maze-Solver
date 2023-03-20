#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "board.h"
#include "menu.h"
#include "error_handler.h"
#include "cli_mode.h"

#define SIZE_COL 8
#define SIZE_ROW 7
#define TEMPLATE_BOARD "boards/template.txt"

// Functions Declarations
bool checkArgumentsExists(int);
void identifyArguments(int, char**);

int main(int argc, char* argv[]) {

	/* If user initialize the program passing arguments, identify which was and if it has valid form.
	 * Else, initialize program with the default execution.
	 */
	if(checkArgumentsExists(argc)) {
		identifyArguments(argc, argv);
	} else {
		Board* currentBoard;
		currentBoard = createCustomBoard(TEMPLATE_BOARD, SIZE_ROW, SIZE_COL);
		playGame(currentBoard);
	}

	return 0;
}

// Checks if user initialize the program passing any argument
bool checkArgumentsExists(int parametersSize) {
	return (parametersSize >= 2);
}

// Identify the commands passed by the user
void identifyArguments(int parametersSize, char* parameters[]) {

	char* command = parameters[1];

	if(strcmp(command, "custom-board") == 0)
		customBoard(parametersSize, parameters);
	else if(strcmp(command, "-h") == 0)
		showListOfCommands();
	else
		invalidCommandError();
}
