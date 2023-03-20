#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "file_handler.h"
#include "board.h"
#include "error_handler.h"

// Clean terminal
void cleanTerminal() {
	system("clear");
}

// Flushes all of stdin buffered data
void flushStdin() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

// Handles all of games interface mechanics
void playGame(Board* currentBoard) {

	char direction;
	char* movements;
	int movementsSize = 1;

	movements = (char*) calloc(movementsSize, sizeof(char));
	checkNullPointer((void*) movements);
	
	while(!gameOver(currentBoard) && !playerHasWon(currentBoard)) {

		cleanTerminal();
		printBoard(currentBoard);

		puts("");

		printNextMovements(currentBoard);

		puts("");

		printf("Movements: ");

		// Prints List of movements
		for(int i = 0; i < movementsSize - 1; i++)
			printf("%c ", movements[i]);
		
		puts("");
		
		printf("\nNext Move[w/a/s/d]: ");
		direction = getchar();

		flushStdin();

		if(direction == 'w' || direction == 'a'|| direction == 's'|| direction == 'd') {
			if(isValidMove(currentBoard, direction)) {
				int* newPosition = playerNewPosition(currentBoard, direction);

				movements[movementsSize - 1] = direction;
				movementsSize++;

				movements = (char*) realloc(movements, movementsSize * sizeof(char));
				checkNullPointer((void*) movements);
				
				updateBoard(currentBoard, newPosition[0], newPosition[1]);
				free(newPosition);
			}
		}
	}

	if(gameOver(currentBoard))
		printf("\nYou Lost :(\n");
	else {
		printf("\nYou Won :)\n");
		saveBoardInfo(movements, movementsSize - 1);
	}
}
