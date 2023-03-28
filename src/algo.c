#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>
#include "menu.h"
#include "board.h"
#include "moviments.h"
#include "error_handler.h"
#include "file_handler.h"
#include "algo.h"

#define MAX_PATHS 1
#define DIST_DIFF_MIN .3
#define DIST_DIFF_MAX 4

// Global variable used for memoization
double* distanceTable;

// Creates a new instance of the Info type
Info* createInfo(Board* currentBoard, MovimentVec* moviments) {

	Info* newInfo = (Info*) malloc(sizeof(Info));

	checkNullPointer((void*) newInfo);

	newInfo -> currentBoard = currentBoard;
	newInfo -> moviments = moviments;

	return newInfo;
}

// Creates a copy from the current struct and returns it's pointer
Info* copyInfo(Info* currentInfo) {

	Info* newInfo;
	Board* newBoard = copyBoard(currentInfo -> currentBoard);
	MovimentVec* newMovimentVec = copyMovimentVec(currentInfo -> moviments);
		
	checkNullPointer((void*) newBoard);
	checkNullPointer((void*) newMovimentVec);

	newInfo = createInfo(newBoard, newMovimentVec);
	return newInfo;
}

// Frees all memory allocated to info struct
void freeInfo(Info* currentInfo) {
	freeBoard(currentInfo -> currentBoard);
	freeMovimentVec(currentInfo -> moviments);
	free(currentInfo);
}

// Initiate the distance table used for memoization
static inline void initDistanceTable(int sizeRow, int sizeCol) {
	distanceTable = calloc(sizeRow * sizeCol, sizeof(double));
	checkNullPointer((void*) distanceTable);
}

// Generates Unique Hash Key
static inline int generateKey(int i, int j, int sizeRow){
	return (i * sizeRow) + j;
}

// Checks if the element represented by the index i, j is already present in the table
static inline bool isInTable(int key) {
	return (distanceTable[key]) ? true : false;
}

// Return the value of the current key
static inline double returnElement(int key) {
	return distanceTable[key];
}

// Add a new value to the key
static inline void addElement(int key, double value) {
	distanceTable[key] = value;
}

// Calculates the player distance to the finish square
double distanceToFinish(Board* currentBoard) {

	int playerPositionX = currentBoard -> player.coordinate[0];
	int playerPositionY = currentBoard -> player.coordinate[1];
	int sizeCol = currentBoard -> sizeCol - 1;
	int sizeRow = currentBoard -> sizeRow - 1;
	int key = generateKey(playerPositionX, playerPositionY, sizeRow);
	
	if(isInTable(key))
		return returnElement(key);
	else {
		double value = sqrt(pow(((double) sizeCol - playerPositionX), 2.0) + pow(((double) sizeRow - playerPositionY), 2.0));
		addElement(key, value);
	}

	return returnElement(key);
}

// Auxiliar function that discover if player can win in the next move
static inline bool gameWonNextMove(NextMoves* nextMoviments) {
	return (nextMoviments -> up == GAME_WON || nextMoviments -> down == GAME_WON || nextMoviments -> right == GAME_WON || nextMoviments -> left == GAME_WON);
}

// Auxiliar functiona that returns if a player is gonna be traped in the next move
static inline bool gameOverNextMove(NextMoves* nextMoviments) {
	return (nextMoviments -> up == GAME_OVER && nextMoviments -> down == GAME_OVER && nextMoviments -> right == GAME_OVER && nextMoviments -> left == GAME_OVER);
}

// Count the possibles next moves a player may have
int countNextMoves(NextMoves* nextMoviments) {

	int count = 0;

	if(nextMoviments -> up != GAME_OVER && nextMoviments -> up != IMPOSSIBLE)
		count++;

	if(nextMoviments -> down != GAME_OVER && nextMoviments -> down != IMPOSSIBLE)
		count++;

	if(nextMoviments -> left != GAME_OVER && nextMoviments -> left != IMPOSSIBLE)
		count++;

	if(nextMoviments -> right != GAME_OVER && nextMoviments -> right != IMPOSSIBLE)
		count++;

	return count;
	
}

// Updates the board and adds the moviment made to the moviment vector
void auxDiscoverOptimalPath(Info* currentInfo, enum MovimentStates currentMove) {

	int* playerCoordinates = playerNewPosition(currentInfo -> currentBoard, currentMove);

	addToMovimentVec(currentInfo -> moviments, currentMove);
	updateBoard(currentInfo -> currentBoard, playerCoordinates[0], playerCoordinates[1]);
	free(playerCoordinates);
}

// Free alocated memory for head node
void freePathsHead(Paths** head) {

	Paths* nextNode;
	Paths* aux = *head;

	if(!aux -> next)
		nextNode = NULL;
	else
		nextNode = aux -> next;

	freeInfo(aux -> currentInfo);
	free(*head);

	*head = nextNode;
}

// Free all paths nodes list
void freePaths(Paths** head) {

	Paths* nextNode, *aux = *head;
	
	while(aux) {
		nextNode = aux -> next;
		freeInfo(aux -> currentInfo);
		free(aux);
		aux = nextNode;
	}
	*head = NULL;
}

// Prints all Paths in the list
void printPaths(Paths* head) {

	Paths* aux = head;

	while(aux) {
		printBoard(aux -> currentInfo ->  currentBoard);
		printf("Moviments: ");
		printMoviments(aux -> currentInfo -> moviments);
		puts("");
		printf("Distance to Finish: %f", aux -> distToFinish);
		puts("\n");

		aux = aux -> next;
	}	
}

// Adds a new path to the list
void addPath(Paths** head, Info* currentInfo) {

	NextMoves* nextMoviments = getNextMoviments(currentInfo -> currentBoard);
	checkNullPointer((void*) nextMoviments);
	
	if(!gameOverNextMove(nextMoviments)) {

		Paths* newPath = (Paths*) malloc(sizeof(Paths));
		checkNullPointer((void*) newPath);

		newPath -> currentInfo = currentInfo;
		newPath -> distToFinish = distanceToFinish(currentInfo -> currentBoard);
		newPath -> totalElements = 0;
		newPath -> next = NULL;
		
		if(*head) {
	 		Paths* aux = *head;
			int i = 0;
			NextMoves* auxNextMoves = NULL;
			bool biggerThanHead = false;
			float distDiff = aux -> distToFinish - newPath -> distToFinish;
			
			if(aux -> distToFinish > newPath -> distToFinish && distDiff < DIST_DIFF_MAX) {
				auxNextMoves = getNextMoviments(aux -> currentInfo -> currentBoard);
				
				if(distDiff > DIST_DIFF_MIN) {
					newPath -> next = aux;
					*head = newPath;
					newPath -> totalElements++;
					biggerThanHead = true;
				} else {
					if(gameWonNextMove(nextMoviments)) {
						newPath -> next = aux;
						*head = newPath;
						newPath -> totalElements++;
						biggerThanHead = true;
					}  else if(countNextMoves(nextMoviments) > countNextMoves(auxNextMoves)) {
						newPath -> next = aux;
						*head = newPath;
						newPath -> totalElements++;
						biggerThanHead = true;
					}
				}
				free(auxNextMoves);
				auxNextMoves = NULL;
			}

			if(!biggerThanHead) {

				if(auxNextMoves)
					free(auxNextMoves);
				
				while(aux -> next && i < MAX_PATHS) {
					if(aux -> next -> distToFinish > newPath -> distToFinish) {
						auxNextMoves = getNextMoviments(aux -> currentInfo -> currentBoard);
						distDiff = aux -> next -> distToFinish - newPath -> distToFinish;

						if(distDiff > DIST_DIFF_MIN){
							newPath -> next = aux -> next;
							aux -> next = newPath;
							free(auxNextMoves);
							break;
						} else {
							if(gameWonNextMove(nextMoviments)){
								newPath -> next = aux -> next;
								aux -> next = newPath;
								free(auxNextMoves);
								break;
							} else if(countNextMoves(nextMoviments) > countNextMoves(auxNextMoves)) {
								newPath -> next = aux -> next;
								aux -> next = newPath;
								free(auxNextMoves);
								break;
							}
							if(auxNextMoves)
								free(auxNextMoves);
						}
					}
					aux = aux -> next;
					i++;
				}

				if(i < MAX_PATHS) {
					if(!aux -> next)
						aux -> next = newPath;

					aux = *head;
					aux -> totalElements++;

					if(aux -> totalElements > MAX_PATHS) {
						i = 0;
						while(i < MAX_PATHS) {
							aux = aux -> next;
							i++;
						}
						freePaths(&aux -> next);
					}
					
				} else {
					freeInfo(currentInfo);
					free(newPath);
				}
			} //else
				//free(newPath);
		
		} else {
			*head = newPath;
			newPath -> totalElements++;
		}
	} else
		freeInfo(currentInfo);

	free(nextMoviments);
}

// First call to search for paths, discovers all possibles paths recursivily and adds it to the list
void firstSearch(Paths** newPaths, Info* currentInfo, int depth) {
	
	if(depth == 0){
		addPath(newPaths, copyInfo(currentInfo));
	} else {
		Info* newInfo = NULL;
		NextMoves* nextUpdate = getNextMoviments(currentInfo -> currentBoard);
		
		if(playerHasWon(currentInfo -> currentBoard)) {
			addPath(newPaths, copyInfo(currentInfo));
			freeInfo(currentInfo);
		}

		if(nextUpdate -> up != IMPOSSIBLE && nextUpdate -> up != GAME_OVER) {
			Info* newInfoUp = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfoUp, UP);

			//printf("Thread %d of %d\n", omp_get_thread_num(), omp_get_max_threads());
			firstSearch(newPaths, newInfoUp, depth - 1);

			if(newInfoUp)
				freeInfo(newInfoUp);
		} else if(nextUpdate -> up == GAME_WON) {
			newInfo = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfo, UP);
			addPath(newPaths, copyInfo(newInfo));
			freeInfo(newInfo);
		}

		if(nextUpdate -> down != IMPOSSIBLE && nextUpdate -> down != GAME_OVER) {
			Info* newInfoDown = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfoDown, DOWN);

			//printf("Thread %d of %d\n", omp_get_thread_num(), omp_get_max_threads());
			firstSearch(newPaths, newInfoDown, depth - 1);
				
			if(newInfoDown)
				freeInfo(newInfoDown);
		} else if(nextUpdate -> down == GAME_WON) {
			newInfo = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfo, DOWN);
			addPath(newPaths, copyInfo(newInfo));
			freeInfo(newInfo);
		}

		if(nextUpdate -> left != IMPOSSIBLE && nextUpdate -> left != GAME_OVER) {
			Info* newInfoLeft = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfoLeft, LEFT);

			//printf("Thread %d of %d\n", omp_get_thread_num(), omp_get_max_threads());
			firstSearch(newPaths, newInfoLeft, depth - 1);
			
			if(newInfoLeft)
				freeInfo(newInfoLeft);
		} else if(nextUpdate -> left == GAME_WON) {
			newInfo = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfo, LEFT);
			addPath(newPaths, copyInfo(newInfo));
			freeInfo(newInfo);
		}
		
		if(nextUpdate -> right != IMPOSSIBLE && nextUpdate -> right != GAME_OVER) {
			Info* newInfoRight = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfoRight, RIGHT);

			//printf("Thread %d of %d\n", omp_get_thread_num(), omp_get_max_threads());
			firstSearch(newPaths, newInfoRight, depth - 1);

			if(newInfoRight)
				freeInfo(newInfoRight);
		} else if(nextUpdate -> up == GAME_WON) {
			newInfo = copyInfo(currentInfo);
			auxDiscoverOptimalPath(newInfo, RIGHT);
			addPath(newPaths, copyInfo(newInfo));
			freeInfo(newInfo);
		}
		free(nextUpdate);
	}
}

// Continue searching for an win path but now with the result of the previous searchs
void continuosSearch(Paths** prevResults, Paths** newPaths, int depth) {

	Paths* auxPrevResults = *prevResults;
	int totalSize = auxPrevResults -> totalElements;

    #pragma omp parallel private(auxPrevResults)
	{
		auxPrevResults = *prevResults;
		
        #pragma omp for
		for(int i = 0; i < totalSize; i++)/*(auxPrevResults)*/ {
			
			firstSearch(newPaths, auxPrevResults -> currentInfo, depth);
		 
			auxPrevResults = auxPrevResults -> next;
		}

		printf("Thread %d of %d\n", omp_get_thread_num(), omp_get_max_threads());
	}
}

// Deep copies a list to another
void copyPaths(Paths** src, Paths** target) {

	Paths* auxSrc = *src;
	
	while(auxSrc) {
		addPath(target, copyInfo(auxSrc -> currentInfo));
		auxSrc = auxSrc -> next;
	}
}

// Autoplay through the entire maze and writes the winning path to a file
void autoPlay(Board* currentBoard, int depth) {

	MovimentVec* moviments = initMovimentVec();
	Info* currentInfo = createInfo(currentBoard, moviments);
	Paths* head = NULL;
	Paths* results = NULL;
	
	initDistanceTable(currentBoard -> sizeRow, currentBoard -> sizeCol);

    //#pragma omp parallel
	//{
	firstSearch(&results, copyInfo(currentInfo), depth);
		//
	printf("\nMoviments: ");
	printMoviments(results -> currentInfo -> moviments);
	printf("Total Elements in Moviment Vec: %ld\n", results -> currentInfo -> moviments -> totalElements);
	printf("Total Elements in LinkedList: %d\n\n", results -> totalElements);
	
	while(!gameOver(currentInfo -> currentBoard) && !playerHasWon(currentInfo -> currentBoard)) {

		freePaths(&head);
		continuosSearch(&results, &head, depth);

		if(!head){
			printNextMoviments(currentInfo -> currentBoard);
			break;
		}
		
		freePaths(&results);
		copyPaths(&head, &results);

		//printBoard(currentInfo -> currentBoard);
		printf("\nMoviments: ");
		printMoviments(results -> currentInfo -> moviments);
		printf("Total Elements in Moviment Vec: %ld\n", results -> currentInfo -> moviments -> totalElements);
		printf("Total Elements in LinkedList: %d\n\n", results -> totalElements);

		free(currentInfo);
		currentInfo = copyInfo(results -> currentInfo);
	}
	
	freePaths(&results);
	freePaths(&head);
	
	if(gameOver(currentInfo -> currentBoard))
		printf("The Algorithm Lost The Game! :(\n");
	else if(playerHasWon(currentInfo -> currentBoard)) {
		printf("The Algorithm Has Found a Correct Path! :)\n");
		saveBoardInfo(currentInfo -> moviments);
	} else
		deadEndPath();
	

	free(distanceTable);
	freeInfo(currentInfo);
}
