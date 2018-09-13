CC=g++
CFLAGS=-Wall -Wextra -pedantic -c
LFLAGS=-I.
OBJS=main.o driver.o
EXE=modeltoworld

all: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(EXE)

main.o: main.cpp 
	$(CC) $(CFLAGS) main.cpp

driver.o: driver.cpp driver.h
	$(CC) $(CFLAGS) driver.cpp

clean: 
	rm -rf $(OBJS) $(EXE)
