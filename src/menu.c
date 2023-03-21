#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "file_handler.h"
#include "moviments.h"
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

// Auxiliar Function to printNextMovement
void printNextMovementsAux(enum MovimentStates movement) {
	switch(movement) {
	    case IMPOSSIBLE:
			printf("Impossible\n");
			break;

		case OKAY:
			printf("Okay\n");
			break;

		case GAME_OVER:
			printf("Game Over\n");
			break;

		case GAME_WON:
			printf("Game Won\n");
			break;

		case START_PIECE:
			printf("Start Piece\n");
			break;
	}
}

// Prints all possibles next moves and it's results
void printNextMoviments(Board* currentBoard) {

	NextMoves* moviments = getNextMoviments(currentBoard);
	
	printf("Up: ");
	printNextMovementsAux(moviments -> up);

	printf("Down: ");
	printNextMovementsAux(moviments -> down);

	printf("Left: ");
	printNextMovementsAux(moviments -> left);

	printf("Right: ");
	printNextMovementsAux(moviments -> right);

	free(moviments);
}

// Prints the current board to the screen
void printBoard(Board* currentBoard) {

	Piece** boardPieces = currentBoard -> pieces;
	int playerPositionX = currentBoard -> player.coordinate[0];
	int playerPositionY = currentBoard -> player.coordinate[1];
	
	for(int i = 0; i < currentBoard -> sizeRow; i++) {
		for(int j = 0; j < currentBoard -> sizeCol; j++) {

			if(playerPositionY == i && playerPositionX == j) {
				printf("> ");
				continue;
			}
			
			switch(boardPieces[i][j].state) {

				case DEAD:
					printf("◻ ");
					break;

				case ALIVE:
					printf("▨ ");
					break;

				case START:
					printf("▣ ");
					break;

				case FINISH:
					printf("▣ ");
					break;
			}
		}
		puts("");
	}	
}

// Prints all directions inputs presents in the list
void printDirections(MovimentVec* moviments) {

	enum MovimentType* movimentsArr = moviments -> moves;

	printf("Directions: ");
	
	for(int i = 0; i < moviments -> totalElements; i++){
		if(i != moviments -> totalElements - 1)
			printf("%c ", movimentsArr[i]);
		else
			printf("%c", movimentsArr[i]);
	}
}

// Handles all of games interface mechanics
void playGame(Board* currentBoard) {

	char direction;
	MovimentVec* moviments = initMovimentVec();
	
	while(!gameOver(currentBoard) && !playerHasWon(currentBoard)) {

		cleanTerminal();
		printBoard(currentBoard);

		puts("");

		printNextMoviments(currentBoard);

		puts("");

		printDirections(moviments);
		
		puts("");

		printf("\nDirections Size: %ld\n", moviments -> movesSize);
		printf("Directions Total Elements: %ld\n", moviments -> totalElements);
		printf("Directions Vec Size: %ld\n", sizeof(*moviments));
		printf("Last Direction: %c", moviments -> moves[moviments -> totalElements - 1]);
		
		puts("");
			
		printf("\nNext Move[w/a/s/d]: ");
		direction = getchar();

		flushStdin();

		if(direction == 'w' || direction == 'a'|| direction == 's'|| direction == 'd') {
			if(isValidMove(currentBoard, direction)) {
				int* newPosition = playerNewPosition(currentBoard, direction);

				addToMovimentVec(moviments, convertCharToMove(direction));
				
				updateBoard(currentBoard, newPosition[0], newPosition[1]);
				free(newPosition);
			}
		}
	}

	if(gameOver(currentBoard))
		printf("\nYou Lost :(\n");
	else {
		printf("\nYou Won :)\n");
		saveBoardInfo(moviments);
	}

	free(moviments);
}
