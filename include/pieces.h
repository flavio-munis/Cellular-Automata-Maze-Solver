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
Board* createCustomBoard(char*,int,int);

#endif
