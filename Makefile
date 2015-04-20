VPATH = src
CFLAGS = -Wall -Werror -ansi -pedantic

all: rshell

rshell: bin
	g++ rshell.cpp -o rshell

bin:
	mkdir bin
