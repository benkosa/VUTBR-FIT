#
# FILE: proj2.c
# AUTOR: BENJAMIN KOSA
# LOGIN: XKOSAB00
# DATUM: 26.4.2018
# PROJEKT: THE SENATE BUS PROBLEM
# PREDMET: IOS
#

CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -lrt
LFLAGS=-lpthread
BIN=proj2
SOURCE=proj2.c

all:
	$(CC) $(CFLAGS) $(SOURCE) -o $(BIN) $(LFLAGS)

run: all
	./$(BIN)

clean:
	rm $(BIN)
