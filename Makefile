#Copyright Dragomir Andrei-Mihai
# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=my_octave

build: $(TARGETS)

my_octave: main.c functions_matrix.c
	$(CC) $(CFLAGS) -Wall -Wextra main.c functions_matrix.c -o my_octave

pack:
	zip -FSr 312CA_Dragomir_Andrei_Tema2.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
