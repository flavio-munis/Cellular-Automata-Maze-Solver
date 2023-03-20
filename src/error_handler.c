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
