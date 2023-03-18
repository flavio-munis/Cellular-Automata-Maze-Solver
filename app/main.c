#include <stdio.h>
#include <stdlib.h>
#include "pieces.h"

#define SIZE_COL 8
#define SIZE_ROW 7
#define TEMPLATE_BOARD "boards/template.txt"

int main(int argc, char* argv[]) {

	int sizeCol, sizeRow;
	char* boardConfig = NULL;
	Board* board = NULL;
	
	if(argc > 4 || (argc < 4 && argc >= 2)) {
		puts("Too many or Too Few Arguments!\nArguments for Custom Files Should Entered as Follow:\n./desafio_stone [path_to_custom_board_file] [rows] [columns]");
		exit(1);
	} else if(argc == 4) { 
		boardConfig = argv[1];
		sizeRow = atoi(argv[2]);
		sizeCol = atoi(argv[3]);
	} else {	
		boardConfig = TEMPLATE_BOARD;
		sizeRow = SIZE_ROW;
		sizeCol = SIZE_COL;
	}

	board = createCustomBoard(boardConfig, sizeRow, sizeCol);

	printBoard(board);
	//debugBoard(board);

	return 0;
}
