#ifndef PIECES_H_FILE
#define PIECES_H_FILE

enum PieceType {
	CORNER = 3,
	WALL = 5,
	NORMAL = 8,
};

enum PieceState {
	DEAD,
	ALIVE,
	START,
	FINISH
};

typedef struct piece {
	enum PieceType type;
	enum PieceState state;
} Piece;

Piece* createPiece(enum PieceType, enum PieceState);
enum PieceType findType(int, int, int, int);
enum PieceState findState(int, int, int, int);

#endif
