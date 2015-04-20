VPATH = src
CFLAGS = -Wall -Werror -ansi -pedantic

all: rshell bin

rshell: rshell.cpp
	g++ rshell.cpp -o rshell

bin:
	mkdir bin
