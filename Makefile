VPATH = src
CFLAGS = -Wall -Werror -ansi -pedantic

all: rshell bin

rshell: rshell.cpp
	g++ -o $@ -c $<

bin:
	mkdir bin
