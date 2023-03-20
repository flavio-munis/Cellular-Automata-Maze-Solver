#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "board.h"

void cleanTerminal() {
	system("clear");
}

void flushStdin() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void playGame(Board* currentBoard) {

	char direction;
	char* movements;
	int movementsSize = 1;

	movements = (char*) malloc(movementsSize * sizeof(char));
	
	while(!gameOver(currentBoard) && !playerHasWon(currentBoard)) {

		cleanTerminal();
		printBoard(currentBoard);

		puts("");

		printNextMovements(currentBoard);

		puts("");

		printf("Movements: ");
		
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
				
				updateBoard(currentBoard, newPosition[0], newPosition[1]);
				free(newPosition);
			}
		}
	}
}
