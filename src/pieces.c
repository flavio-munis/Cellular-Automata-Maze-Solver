#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pieces.h"

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
