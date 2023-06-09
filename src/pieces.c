#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pieces.h"
#include "error_handler.h"

// Creates a new piece and config it's attributes
void configPiece(Piece* currentPiece, enum PieceType type, enum PieceState state) {
	currentPiece -> type = type;
	currentPiece -> state = state;
}

// Copies all piece information to a new instance
//Piece* copyPiece(Piece* currentPiece) {
//	Piece* newPiece = createPiece(currentPiece -> type, currentPiece -> state);
//	return newPiece;
//}

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
