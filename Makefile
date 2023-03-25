# MakeFile For Stone Programming Languages

# Project Name
PROJECT_NAME = desafio_stone

# Compilation Options
CC = gcc
CC_FLAGS =  -c \
	-I $(INCLUDE) \
	-Wall \
	-pedantic \
	-g \
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

all: ${OBJ} $(PROJECT_NAME) $(RESULTS) project_descrip

$(RESULTS):
	@ mkdir results
	@ echo "Results Folder Created"

$(OBJ):
	@ mkdir obj
	@ echo "Object Folder Created"

$(PROJECT_NAME): $(OBJ_SOURCE) $(MAIN_OBJ)
	@ echo 'Compiling Executable File...'
	@ $(CC) $^ -lm -o $@ -O2
	@ echo 'Finished Building Project!'

$(OBJ)/%.o: $(SRC)/%.c $(INCLUDE)/%.h
	@ echo 'Building and Linking File: $@'
	@ $(CC) $< $(CC_FLAGS) $@
	@ echo ' '

$(OBJ)/main.o: $(APP)/main.c
	@ echo 'Building and Linking Main File: $@'
	@ $(CC) $< $(CC_FLAGS) $@
	@ echo ' '

clean: clean_obj clean_results clean_core

clean_obj:
	@ echo "Cleaning All Object Files..."
	@ rm -rf obj/

clean_results:
	@ echo "Cleaning All Result Files..."
	@ rm -f results/*

clean_core:
	@ echo "Cleaning All Core Files..."
	@ rm -f ./core*

run:
ifdef ARGS
	@ ./${PROJECT_NAME} ${ARGS}
else
	@ ./${PROJECT_NAME}
endif

project_descrip:
	@ echo ' '
	@ echo 'To run this program in gameplay mode just execute the binary.'
	@ echo 'For more info on cli commands please execute the binary using the -h command.'
	@ echo ' '
	@ echo 'Current solution to problem was made using depth=5 using the autoplay command. Algorithm still in beta and not fully optimized or free from bugs'
	@ echo ''
	@ echo 'Had a lot of fun (and hate) moments building this program, really was a opportunity to develop my C programming skills, hope you have fun and remenber that the project is always open for advices and bug fixes :)'
	@ echo ''
