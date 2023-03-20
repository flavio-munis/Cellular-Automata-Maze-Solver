# MakeFile For Stone Programming Languages

# Project Name
PROJECT_NAME = desafio_stone

# Compilation Options
CC = gcc
CC_FLAGS =  -c \
	-I $(INCLUDE) \
	-Wall \
	-pedantic \
	-o

# Folders
SRC = ./src
APP = ./app
OBJ = ./obj
INCLUDE = ./include
RESULTS = ./results

# Files
MAIN = ${APP}/main.c
MAIN_OBJ = ${OBJ}/main.o
C_HEADERS = $(wildcard ${INCLUDE}/*.h)
C_SOURCE = $(wildcard ${SRC}/*.c)
OBJ_SOURCE = $(subst .c,.o,$(subst $(SRC),$(OBJ), $(C_SOURCE)))

all: ${OBJ} $(PROJECT_NAME) ${RESULTS}

$(RESULTS):
	@ mkdir results
	@ echo "Results Folder Created"

$(OBJ):
	@ mkdir obj
	@ echo "Object Folder Created"

$(PROJECT_NAME): $(OBJ_SOURCE) $(MAIN_OBJ)
	@ echo 'Compiling Executable File...'
	@ $(CC) $^ -o $@
	@ echo 'Finished Building Project!'

$(OBJ)/%.o: $(SRC)/%.c $(INCLUDE)/%.h
	@ echo 'Building and Linking File: $@'
	@ $(CC) $< $(CC_FLAGS) $@
	@ echo ' '

$(OBJ)/main.o: $(APP)/main.c
	@ echo 'Building and Linking Main File: $@'
	@ $(CC) $< $(CC_FLAGS) $@
	@ echo ' '

clean:
	@ echo "Cleaning All Object Files..."
	@ rm -rf obj/

clean_results:
	@ echo "Cleaning All Result Files..."
	@ rm -f results/*

run:
ifdef ARGS
	@ ./${PROJECT_NAME} ${ARGS}
else
	@ ./${PROJECT_NAME}
endif
