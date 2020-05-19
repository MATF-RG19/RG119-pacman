GO = pacman
CC = gcc
FLAG = -lm
OGL = -lGL -lGLU -lglut

$(GO): gameplay.o 
	$(CC) -o $@ $^ $(FLAG) $(OGL)

main.o: gameplay.c 
	$(CC) -o $@ $< -c

.PHONY:
	clean

clean:
	rm -rf $(GOAL) *.o
