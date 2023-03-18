#include <stdio.h>
#include <stdlib.h>
#include "pieces.h"

// Prints the current board to the screen
void printBoard(Board* currentBoard) {

	for(int i = 0; i < currentBoard -> sizeRow; i++) {
		for(int j = 0; j < currentBoard -> sizeCol; j++) {
			if(currentBoard -> pieces[i][j].state == DEAD)
				printf("◻");
			else
			printf("▧");
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

	// Verifies if it's the start ou finish piece
	if(i == 0 && j == 0)
		return START;

	if(i == sizeRowAcess && j == sizeColAcess)
		return FINISH;

	// Verifies which type of piece it's based on the current position in the board
	if(i == 0 || i == sizeRowAcess) {
		if(j == 0 || j == sizeColAcess)
			return CORNER_PIECE;
		else
			return WALL_PIECE;
	}

	if(j == 0 || j == sizeColAcess)
		return WALL_PIECE;
	else
		return NORMAL_PIECE;
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
			newBoard -> pieces[i][j] = *createPiece(findType(i,j,sizeRow - 1,sizeCol - 1), DEAD);
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

			printf("%s ", (boardPieces[i][j].state == DEAD) ? "Dead" : "Alive");

			switch(boardPieces[i][j].type) {

			    case NORMAL_PIECE:
					printf("Normal Piece");
					break;

				case WALL_PIECE:
					printf("Wall Piece");
					break;

				case CORNER_PIECE:
					printf("Corner Piece");
					break;

				case START:
					printf("Start Piece");
					break;

				case FINISH:
					printf("Finish Piece");
					break;

			    default:
					printf("Unknow Piece Type");
			}

			if(x == j && y == i)
				printf(" * Player is Here\n");
			else
				puts("");
			
		}
		puts("");
	}
	
}

//void createCustomBoard(int** )
