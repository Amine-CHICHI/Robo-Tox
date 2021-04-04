#CC on compile avec le gcc
CC = gcc

# -w supprime la majorite des avertissements et les flags de compilation
COMPILER_FLAGS = `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lm -w

#Le linkage avec les biblio SDl 
LINKER_FLAGS = -lSDL2

#Nom de notre executable 
OBJ_NAME = Robot

#La cible qui generera notre executable 
all : fonctions.o main.o
	$(CC) main.o fonctions.o $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)


functions.o: fonctions.c
	$(CC) -c fonctions.c $(COMPILER_FLAGS) $(LINKER_FLAGS)


main.o: main.c fonctions.h
	$(CC) -c main.c $(COMPILER_FLAGS) $(LINKER_FLAGS)

clear:
	rm *.o
