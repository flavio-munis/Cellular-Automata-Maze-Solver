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

#define MAX_PATHS 3
#define MAX_GAMES 10000

// Global variables used for memoization
double* distanceTable;
PathsTable** pathsMemo;

// Calculates the player distance to the finish square
double distanceToFinish(Board* currentBoard);

// Init the paths table global variable
void initPathsTable(Board* currentBoard) {

	float maxDist = distanceToFinish(currentBoard);

	pathsMemo = calloc(ceil(maxDist) + 1, sizeof(PathsTable*));
	checkNullPointer((void*) pathsMemo);
}

// Adds a new node to paths table
void addToPathTable(MovimentVec* moviments, float playerDist, float score) {

	int key = ceil(playerDist);
	PathsTable* newNode = (PathsTable*) malloc(sizeof(PathsTable));
	checkNullPointer((void*) newNode);

	newNode -> score = score;
	newNode -> moviments = moviments;
	newNode -> next = NULL;
	
	if(!pathsMemo[key])
		pathsMemo[key] = newNode;
	else {
		PathsTable* aux = pathsMemo[key];
		
		while(aux -> next)
			aux = aux -> next;

		aux -> next = newNode;
	}
}

// Free all nodes from paths table
void freePathTable(Board* defaultBoard) {

	int maxDist = ceil(distanceToFinish(defaultBoard));
	
	for(int i = 0; i < maxDist; i++) {

		if(pathsMemo[i]) {

			PathsTable* aux = pathsMemo[i], *nextNode;

			while(aux) {
				nextNode = aux -> next;
				freeMovimentVec(aux -> moviments);
				free(aux);
				aux = nextNode;
			}
		}
	}
}

// Gets a node from table if it exists, or returns null if it doesn't
PathsTable* getPathFromTable(MovimentVec* moves, float playerDist) {

	int key = ceil(playerDist);

	if(pathsMemo[key]) {
		PathsTable* head = pathsMemo[key];
		PathsTable* aux = head;

		while(aux) {
			if(compareMovimentVecs(moves, aux -> moviments))
				return aux;
			
			aux = aux -> next;
		}
	}

	return NULL;
}

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
void freeInfo(Info** currentInfo) {
	
	freeBoard((*currentInfo) -> currentBoard);
	freeMovimentVec((*currentInfo) -> moviments);
	free(*currentInfo);
	
	*currentInfo = NULL;
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

static inline float calculateScore(float distToFinish, int validNextMoves, Piece* playerPiece) {

	float score = (distToFinish / 10) - (validNextMoves / 10);

	/*switch(playerPiece -> type) {

	    case CORNER:
		    score *= 1.7;
			break;

	    case WALL:
			score *= 1.5;
			break;

	    case NORMAL:
			score /= 1.5;
			break;
	}

	switch(playerPiece -> state) {

	    case START:
		   score *= 1.3;
		   break;

	    case FINISH:
		   score /= 20;
		   break;

	    case ALIVE:
	    case DEAD:
			break;
			}*/
	
	return score;
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

// Free all paths nodes list
void freePaths(Paths** head) {

	Paths* nextNode, *aux = *head;
	
	while(aux) {
		nextNode = aux -> next;
		if(aux -> currentInfo)
			freeInfo(&aux -> currentInfo);
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
		printf("Score: %f", aux -> score);
		puts("\n");

		aux = aux -> next;
	}	
}

// Creates a new path node
Paths* createPath(Info* currentInfo, NextMoves* nextMoviments) {

	Paths* newPath = (Paths*) malloc(sizeof(Paths));

	checkNullPointer((void*) nextMoviments);
	checkNullPointer((void*) newPath);

	int playerPositionX = currentInfo -> currentBoard -> player.coordinate[0];
	int playerPositionY = currentInfo -> currentBoard -> player.coordinate[1];
	Piece* playerPiece = &currentInfo -> currentBoard -> pieces[playerPositionY][playerPositionX];
	
	newPath -> currentInfo = currentInfo;
	newPath -> distToFinish = distanceToFinish(currentInfo -> currentBoard);
	newPath -> score = calculateScore(newPath -> distToFinish, countNextMoves(nextMoviments), playerPiece);
	newPath -> totalElements = 0;
	newPath -> next = NULL;
	
	return newPath;
}

int countElements(Paths** head) {

	Paths* aux = *head;
	int result = 0;
	
	while(aux) {
		aux = aux -> next;
		result++;
	}

	return result;
}

void addPath2(Paths** head, Info* currentInfo) {

	NextMoves* nextMoviments = getNextMoviments(currentInfo -> currentBoard);
	checkNullPointer((void*) nextMoviments);
	
	if(!gameOverNextMove(nextMoviments)) {

		Paths* aux = *head;
		Paths* newPath = createPath(currentInfo, nextMoviments);
		PathsTable* pathNode = getPathFromTable(currentInfo -> moviments, newPath -> distToFinish);
		
		// If path score has already been calculated
		if(pathNode) {
			// pathNode -> score += 0.5;
			newPath -> score = pathNode -> score;	
		} else
			addToPathTable(copyMovimentVec(currentInfo -> moviments), newPath -> distToFinish, newPath -> score);
			
		// If theres no paths in head
		if(*head) {
			
			bool shouldBeAdded = true;

			// Checks if current path score is bigger than the head of the list
			if((*head) -> score > newPath -> score) {
				newPath -> next = aux;
				newPath -> totalElements = (*head) -> totalElements + 1;
				(*head) = newPath;
			} else {
				int i = 0;
				
				// Loops through the list
				while(aux -> next) {
					i++;

					// If the path score is bigger than the worse than the worst node, breaks the loop and dont add the new node
					if(i == MAX_PATHS - 1) {
						shouldBeAdded = false;
						break;
					}

					// If the new node score is greater than some element in the list
					if(aux -> next -> score > newPath -> score) {
						newPath -> next = aux -> next;
						aux -> next = newPath;
						shouldBeAdded = false;
						(*head) -> totalElements++;
						break;
					}
					aux = aux -> next;
				}

				// Adds the element to the end of the list
				if(shouldBeAdded) {
					aux -> next = newPath;
					(*head) -> totalElements++;
				} else {
					if(i == MAX_PATHS - 1){
						freePaths(&aux -> next);
						(*head) -> totalElements = countElements(head);
						freePaths(&newPath);
					}
				}
			}
					
		} else {
			(*head) = newPath;
			newPath -> totalElements++;
		}
	} else {
		freeInfo(&currentInfo);
	}
		
	free(nextMoviments);
}
	
// First call to search for paths, discovers all possibles paths recursivily and adds it to the list
void firstSearch(Paths** newPaths, Info* currentInfo, int depth) {

    if (depth == 0) {
        #pragma omp critical
        {
            addPath2(newPaths, copyInfo(currentInfo));
        }
    } else {
        if (playerHasWon(currentInfo->currentBoard)) {
            #pragma omp critical
            {
                addPath2(newPaths, copyInfo(currentInfo));
            }
        }

        NextMoves* nextUpdate = getNextMoviments(currentInfo->currentBoard);

        #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < 4; i++) {
            Info* newInfo = NULL;

            switch (i) {
                case 0:
                    if (nextUpdate->up != IMPOSSIBLE && nextUpdate->up != GAME_OVER) {
                        newInfo = copyInfo(currentInfo);
                        auxDiscoverOptimalPath(newInfo, UP);
                    }
                    break;
                case 1:
                    if (nextUpdate->down != IMPOSSIBLE && nextUpdate->down != GAME_OVER) {
                        newInfo = copyInfo(currentInfo);
                        auxDiscoverOptimalPath(newInfo, DOWN);
                    }
                    break;
                case 2:
                    if (nextUpdate->left != IMPOSSIBLE && nextUpdate->left != GAME_OVER) {
                        newInfo = copyInfo(currentInfo);
                        auxDiscoverOptimalPath(newInfo, LEFT);
                    }
                    break;
                case 3:
                    if (nextUpdate->right != IMPOSSIBLE && nextUpdate->right != GAME_OVER) {
                        newInfo = copyInfo(currentInfo);
                        auxDiscoverOptimalPath(newInfo, RIGHT);
                    }
                    break;
            }

            if (newInfo) {
                #pragma omp task shared(newPaths)
                {
                    firstSearch(newPaths, newInfo, depth - 1);
                    #pragma omp critical
                    {
                        addPath2(newPaths, copyInfo(currentInfo));
                    }
                    freeInfo(&newInfo);
                }
            }
        }

        free(nextUpdate);
    }
}




// Continue searching for an win path but now with the result of the previous searchs
void continuosSearch(Paths** prevResults, Paths** newPaths, int depth) {

	Paths* auxPrevResults = *prevResults;
	
	while(auxPrevResults) {
			
		firstSearch(newPaths, auxPrevResults -> currentInfo, depth);
		 
		auxPrevResults = auxPrevResults -> next;
	}
}

// Deep copies a list to another
void copyPaths(Paths** src, Paths** target) {

	Paths* auxSrc = *src;
	
	while(auxSrc) {
		
		addPath2(target, copyInfo(auxSrc -> currentInfo));
		auxSrc = auxSrc -> next;
	}
}

Info* findPaths(Info* defaultInfo, int depth) {

	Paths* head = NULL;
	Paths* results = NULL;
	Info* currentInfo = defaultInfo;
	
	firstSearch(&results, copyInfo(currentInfo), depth);
	
	printf("\nMoviments: ");
	printMoviments(results -> currentInfo -> moviments);
	printf("Current Moviments Score: %f\n", results -> score);
	printf("Total Elements in Moviment Vec: %ld\n", results -> currentInfo -> moviments -> totalElements);
	printf("Total Elements in LinkedList: %d\n\n", results -> totalElements);
	
	while(!gameOver(currentInfo -> currentBoard) && !playerHasWon(currentInfo -> currentBoard)) {

		freePaths(&head);
		continuosSearch(&results, &head, depth);

		if(!head) {
			printNextMoviments(currentInfo -> currentBoard);
			puts("");
			break;
		} else {
			if(compareMovimentVecs(results -> currentInfo -> moviments, head -> currentInfo -> moviments)) {
				break;
			}
		}
		
		freePaths(&results);
		results = NULL;
		copyPaths(&head, &results);

		printf("\nMoviments: ");
		printMoviments(results -> currentInfo -> moviments);
		printf("Current Moviments Score: %f\n", results -> score);
		printf("Total Elements in Moviment Vec: %ld\n", results -> currentInfo -> moviments -> totalElements);
		printf("Total Elements in LinkedList: %d\n\n", results -> totalElements);

		currentInfo = copyInfo(results -> currentInfo);
	}
	
	freePaths(&results);
	freePaths(&head);

	return currentInfo;
}

// Autoplay through the entire maze and writes the winning path to a file
void autoPlay(Board* currentBoard, int depth) {

	MovimentVec* moviments = initMovimentVec();
	Info* defaultInfo = createInfo(currentBoard, moviments);
	Info* currentInfo;
	PathsTable* pathNode;
	
	initDistanceTable(currentBoard -> sizeRow, currentBoard -> sizeCol);
	initPathsTable(defaultInfo -> currentBoard);
	
	for(int i = 0; i < MAX_GAMES; i++) {

		currentInfo = findPaths(copyInfo(defaultInfo), depth);
		
		if(gameOver(currentInfo -> currentBoard)) {
			printf("The Algorithm Lost The Game! :(\n");
		} else if(playerHasWon(currentInfo -> currentBoard)) {
			printf("The Algorithm Has Found a Correct Path! :)\n");
			saveBoardInfo(currentInfo -> moviments);
			break;
		} else{
			printf("The Algorithm Couldn't Finish The Game! :(\n");
			pathNode = getPathFromTable(currentInfo -> moviments, distanceToFinish(currentInfo -> currentBoard));
			pathNode -> score += 1;
		}
		
		freeInfo(&currentInfo);
	}

	free(distanceTable);
	freePathTable(defaultInfo -> currentBoard);
	freeInfo(&defaultInfo);
}
