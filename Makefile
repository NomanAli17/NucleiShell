CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
SRC = main.c parser.c executor.c builtins.c history.c jobs.c signals.c
OBJ = $(SRC:.c=.o)

all: nucleishell

nucleishell: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) nucleishell

