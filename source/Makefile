CC=g++
CFLAGS=-Wall -Wextra -pedantic -c
LFLAGS=-I.
OBJS=main.o
EXE=modeltoworld

all: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(EXE)

main.o: main.cpp 
	$(CC) $(CFLAGS) main.cpp

clean: 
	rm -rf $(OBJS) $(EXE)
