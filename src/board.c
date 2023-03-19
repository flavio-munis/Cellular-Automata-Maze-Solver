#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "pieces.h"
#include "file_handler.h"

// Functions Declarations
int checkNeighbours(Board*, int, int);

// Creates a new Board with dynamic size
Board* createBoard(int sizeRow, int sizeCol) {

	Board* newBoard = (Board*) malloc(sizeof(Board));
	Player newPlayer;

	// Iniciates player attribute
	newPlayer.coordinate[0] = 0;
	newPlayer.coordinate[1] = 0;
	
	// Iniciates board attributes
	newBoard -> sizeCol = sizeCol;
	newBoard -> sizeRow = sizeRow;
	newBoard -> player = newPlayer;
	newBoard -> pieces = (Piece**) malloc(sizeRow * sizeof(Piece));
	
	for(int i = 0; i < sizeRow; i++) {

		// Allocates memory for each row
		newBoard -> pieces[i] = (Piece*) malloc(sizeCol * sizeof(Piece));

		// Adds all pieces to the 
		for(int j = 0; j < sizeCol; j++) {
			newBoard -> pieces[i][j] = *createPiece(findType(i,j,sizeRow - 1,sizeCol - 1), findState(i,j,sizeRow - 1, sizeCol - 1));
		}
	}
		   
	return newBoard;
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

// Prints all info about every Piece on the board
void debugBoard(Board* currentBoard) {

	Piece** boardPieces = currentBoard -> pieces;
	int x = currentBoard -> player.coordinate[0];
	int y = currentBoard -> player.coordinate[1];
	int sizeRow = currentBoard -> sizeRow;
	int sizeCol = currentBoard -> sizeCol;
	
	puts("=== Pieces Information ===\n");
	
	for(int i = 0; i < sizeRow; i++) {

		printf("%dº Row: ", i + 1);
		
		for(int j = 0; j < sizeCol; j++) {

			switch(boardPieces[i][j].state) {

				case DEAD:
					printf("Dead");
					break;

				case ALIVE:
					printf("Alive");
					break;

				case START:
					printf("Start");
					break;

				case FINISH:
					printf("Finish");
					break;
			}

			switch(boardPieces[i][j].type) {

			    case NORMAL:
					printf(" Normal Piece");
					break;

				case WALL:
					printf(" Wall Piece");
					break;

				case CORNER:
					printf(" Corner Piece");
					break;

			    default:
					printf(" Unknow Piece Type");
			}

			if(x == j && y == i)
				printf(" * Player is Here\n");
			else
				puts("");
			
		}
		puts("");
	}

	puts("=== Neighbours by Piece ===\n");
	
	for(int i = 0; i < sizeRow; i++) {
		for(int j = 0; j < sizeCol; j++) {
			printf("%d ", checkNeighbours(currentBoard, i, j));
		}
		puts("");
	}
	
}

/* Creates a custom board based on the user parameters
   
   0 - Piece is Dead
   1 - Piece is Alive
   2 - Player
   3 - START
   4 - FINISH

 */
Board* createCustomBoard(char* boardConfig, int sizeRow, int sizeCol) {

	Board* newBoard = createBoard(sizeRow, sizeCol);
	Piece** boardPieces = newBoard -> pieces;
	Player* currentPlayer = &newBoard -> player;
	int** pseudoBoard = readBoardFromFile(boardConfig, sizeRow, sizeCol);
	
	for(int i = 0; i < sizeRow; i++) {
		for(int j = 0; j < sizeCol; j++) {

			switch(pseudoBoard[i][j]) {

				case 0:
					boardPieces[i][j].state = DEAD;
					break;
				
				case 1:
					boardPieces[i][j].state = ALIVE;
					break;
				
			    case 2:
					currentPlayer -> coordinate[0] = j;
					currentPlayer -> coordinate[1] = i;
					break;

				case 3:
					boardPieces[i][j].state = START;
					break;

				case 4:
					boardPieces[i][j].state = FINISH;
					break;
			}			
		}
	}

	// Free memory from pseudoboard
	for(int i = 0; i < sizeRow; i++)
		free(pseudoBoard[i]);

	free(pseudoBoard);

	return newBoard;
}

int checkNeighbours(Board* currentBoard, int i, int j) {

	int sizeRow = currentBoard -> sizeRow;
	int sizeCol = currentBoard -> sizeCol;
	int neighboursAlive = 0;
	Piece** boardPieces = currentBoard -> pieces;

	switch(boardPieces[i][j].type) {
		
	    case CORNER:

			// Left Pieces
			if(i == 0) {
				if(boardPieces[i+1][j].state == ALIVE)
					neighboursAlive++;
			}

			if(i == sizeRow - 1) {
				if(boardPieces[i-1][j].state == ALIVE)
					neighboursAlive++;
			}

			// Right Pieces
			if(j == 0) {
				if(boardPieces[i][j+1].state == ALIVE)
					neighboursAlive++;
			}

			if(j == sizeCol - 1) {
				if(boardPieces[i][j-1].state == ALIVE)
					neighboursAlive++;
			}

			// All 4 Pieces
			if(i == 0 && j == 0) {
				if(boardPieces[i+1][j+1].state == ALIVE)
					neighboursAlive++;
			} else if(i == 0 && j == sizeCol - 1) {
				if(boardPieces[i+1][j-1].state == ALIVE)
					neighboursAlive++;
			} else if(i == sizeRow - 1 && j == 0) {
				if(boardPieces[i-1][j+1].state == ALIVE)
					neighboursAlive++;
			} else if(i == sizeRow - 1 && j == sizeCol - 1) {
				if(boardPieces[i-1][j-1].state == ALIVE)
					neighboursAlive++;
			}
			break;

	    case WALL:

			//Top and Bottom Pieces
			if(i == 0 || i == sizeRow - 1) {
				if(boardPieces[i][j-1].state == ALIVE)
					neighboursAlive++;

				if(boardPieces[i][j+1].state == ALIVE)
					neighboursAlive++;
			}

			// Left and Right Pieces
			if(j == 0 || j == sizeCol - 1) {
				if(boardPieces[i-1][j].state == ALIVE)
					neighboursAlive++;

				if(boardPieces[i+1][j].state == ALIVE)
					neighboursAlive++;
			}
			
			//Top Pieces
			if(i == 0) {
				if(boardPieces[i+1][j-1].state == ALIVE)
					neighboursAlive++;

				if(boardPieces[i+1][j].state == ALIVE)
					neighboursAlive++;

				if(boardPieces[i+1][j+1].state == ALIVE)
					neighboursAlive++;
			}

			//Bottom Pieces
			if(i == sizeRow - 1) {
				if(boardPieces[i-1][j-1].state == ALIVE)
					neighboursAlive++;

				if(boardPieces[i-1][j].state == ALIVE)
					neighboursAlive++;

				if(boardPieces[i-1][j+1].state == ALIVE)
					neighboursAlive++;
			}

			//Left Pieces
			if(j == 0) {
				if(boardPieces[i+1][j+1].state == ALIVE)
					neighboursAlive++;

				if(boardPieces[i][j+1].state == ALIVE)
					neighboursAlive++;

				if(boardPieces[i-1][j+1].state == ALIVE)
					neighboursAlive++;
			}

			//Right Pieces
			if(j == sizeCol - 1) {
				if(boardPieces[i+1][j-1].state == ALIVE)
					neighboursAlive++;

				if(boardPieces[i][j-1].state == ALIVE)
					neighboursAlive++;

				if(boardPieces[i-1][j-1].state == ALIVE)
					neighboursAlive++;
			}
			break;

	    case NORMAL:

			// All Pieces
			if(boardPieces[i+1][j-1].state == ALIVE)
				neighboursAlive++;

			if(boardPieces[i+1][j].state == ALIVE)
				neighboursAlive++;

			if(boardPieces[i+1][j+1].state == ALIVE)
				neighboursAlive++;

			if(boardPieces[i][j+1].state == ALIVE)
				neighboursAlive++;
			
			if(boardPieces[i-1][j+1].state == ALIVE)
				neighboursAlive++;
			
			if(boardPieces[i-1][j].state == ALIVE)
				neighboursAlive++;

			if(boardPieces[i-1][j-1].state == ALIVE)
				neighboursAlive++;

			if(boardPieces[i][j-1].state == ALIVE)
				neighboursAlive++;
			break;
	}

	return neighboursAlive;
}

void updateBoard(Board* currentBoard) {

	int sizeRow = currentBoard -> sizeRow;
	int sizeCol = currentBoard -> sizeCol;
	Board* auxBoard = createBoard(sizeRow,sizeCol);
	Piece** boardPieces = currentBoard -> pieces;
	Piece** auxBoardPieces = auxBoard -> pieces;
	
	for(int i = 0; i < sizeRow; i++) {
		for(int j = 0; j < sizeCol; j++) {
			
			int neighboursAlive = checkNeighbours(currentBoard, i, j);
			
			switch(boardPieces[i][j].type) {

			    case CORNER:

					// Corner pieces don't have enough neighbours to stay alive
					if(boardPieces[i][j].state == ALIVE)
						auxBoardPieces[i][j].state = DEAD;
					else {
						if(neighboursAlive > 2)
							auxBoardPieces[i][j].state = ALIVE;
					}
					break;

			    case WALL:

					// Corner pieces don't have enough neighbours to stay alive
					if(boardPieces[i][j].state == ALIVE) {
						if(neighboursAlive < 4)
							auxBoardPieces[i][j].state = DEAD;
						else
							auxBoardPieces[i][j].state = ALIVE;
					} else {
						if(neighboursAlive == 2 || neighboursAlive == 3)
							auxBoardPieces[i][j].state = ALIVE;
						else
							auxBoardPieces[i][j].state = DEAD;
					}
					break;

				case NORMAL:

					// Corner pieces don't have enough neighbours to stay alive
					if(boardPieces[i][j].state == ALIVE){
						if(neighboursAlive < 4 || neighboursAlive > 6)
							auxBoardPieces[i][j].state = DEAD;
						else
							auxBoardPieces[i][j].state = ALIVE;
					} else{
						if(neighboursAlive == 2 || neighboursAlive == 3)
							auxBoardPieces[i][j].state = ALIVE;
						else
							auxBoardPieces[i][j].state = DEAD;
					}
					break;
			}
		}
	}

	free(boardPieces);
	currentBoard -> pieces = auxBoardPieces;
	free(auxBoard);
}
