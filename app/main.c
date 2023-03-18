#include <stdio.h>
#include <stdlib.h>
#include "pieces.h"

#define SIZE_COL 7
#define SIZE_ROW 8

int main(int argc, char* argv[]) {

	Board* board = createBoard(SIZE_ROW, SIZE_COL);
	
	printBoard(board);
	debugBoard(board);

	return 0;
}
