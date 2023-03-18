#include <stdio.h>
#include <stdlib.h>
#include "pieces.h"
#include "file_handler.h"

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

// Creates a new piece and config it's attributes
Piece* createPiece(enum PieceType type, enum PieceState state) {

	Piece* newPiece = (Piece*) malloc(sizeof(Piece));

	newPiece -> type = type;
	newPiece -> state = state;

	return newPiece;
}

// Find the type of the current piece in the board
enum PieceType findType(int i, int j, int sizeRowAcess, int sizeColAcess) {

	// Verifies which type of piece it's based on the current position in the board
	if(i == 0 || i == sizeRowAcess) {
		if(j == 0 || j == sizeColAcess)
			return CORNER;
		else
			return WALL;
	}

	if(j == 0 || j == sizeColAcess)
		return WALL;
	else
		return NORMAL;
}

enum PieceState findState(int i, int j, int sizeRowAcess, int sizeColAcess) {

	// Verifies if it's the start ou finish piece
	if(i == 0 && j == 0)
		return START;

	if(i == sizeRowAcess && j == sizeColAcess)
		return FINISH;

	return DEAD;
}

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

void debugBoard(Board* currentBoard) {

	Piece** boardPieces = currentBoard -> pieces;
	int x = currentBoard -> player.coordinate[0];
	int y = currentBoard -> player.coordinate[1];

	for(int i = 0; i < currentBoard -> sizeRow; i++) {

		printf("%dº Row: ", i + 1);
		
		for(int j = 0; j < currentBoard -> sizeCol; j++) {

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
