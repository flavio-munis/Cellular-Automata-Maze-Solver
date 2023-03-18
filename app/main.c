#include <stdio.h>
#include <stdlib.h>
#include "pieces.h"

#define SIZE_COL 7
#define SIZE_ROW 8

int main(int argc, char* argv[]) {

	int customConfig[7][8] = {{3,0,0,0,0,0,0,0},
							  {0,0,0,0,1,0,0,0},
							  {0,0,1,0,1,1,0,0},
							  {0,1,1,0,0,1,1,0},
							  {0,0,1,0,1,1,0,0},
							  {0,0,0,0,1,0,0,0},
							  {0,0,0,0,0,0,0,4}};
	
	Board* board = createCustomBoard(customConfig, SIZE_ROW, SIZE_COL);


	printBoard(board);
	debugBoard(board);

	
	return 0;
}
