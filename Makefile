CC=clang-3.7
CFLAGS=-O3 -Wall -Wextra -pedantic -march=native 
LIBS=-lgc
SRCS=$(wildcard ./*.c)

all: bcscm

bcscm: $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) $(LIBS) -o bcscm

	
