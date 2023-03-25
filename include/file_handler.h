#ifndef FILE_HANDLER_HEADER
#define FILE_HANDLER_HEADER

#include "moviments.h"

int** readBoardFromFile(char*, int, int);
void saveBoardInfo(MovimentVec*);
MovimentVec* readMovimentsFromFile(char*);

#endif
