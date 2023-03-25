#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Throws Error if the pointer is null
void checkNullPointer(void* pointer) {
	if(!pointer) {
		printf("Error Value: %d\n", errno);
		perror("Pointer Couldn't be Allocated");
		exit(EXIT_FAILURE);
	}	
}

// Throws error if the command passed to the program is invalid
void invalidCommandError() {
	fprintf(stderr,"Invalid Command!\nFor Complete Command List Please Use -h!\n");
	exit(EXIT_FAILURE);
}

// Throws error if the parameters for command are invalid
void invalidParametersError(char* command, char* message) {
	fprintf(stderr, "Invalid Parameters for Command %s!\nParamaters Should be Entered as Follow:\n\n%s\n", command, message);
	exit(EXIT_FAILURE);
}

// Trows an error when the algorithm is stuck at a dead end
void deadEndPath() {
	fprintf(stderr, "The Algorithm couldn't find a solution to the maze, plese try a different depth or a diffenrent maze!\n");
	exit(EXIT_FAILURE);
}
