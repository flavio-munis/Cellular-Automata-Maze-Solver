#ifndef PIECES_H_FILE
#define PIECES_H_FILE

enum PieceType {
	CORNER_PIECE = 3,
	WALL_PIECE = 5,
	NORMAL_PIECE = 8,
	START = 1,
	FINISH = 2
};

enum PieceState {
	DEAD,
	ALIVE
};

typedef struct piece {
	enum PieceType type;
	enum PieceState state;
} Piece;

typedef struct player {
	int coordinate[2];
} Player;

typedef struct board {
	int sizeCol;
	int sizeRow;
	Piece** pieces;
	Player player;
} Board;

void printBoard(Board*);
void debugBoard(Board*);
Board* createBoard(int, int);

#endif
