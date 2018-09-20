CC=g++
CFLAGS=-Wall -Wextra -pedantic -c
LFLAGS=-I.
OBJS=main.o driver.o model.o
EXE=modeltoworld

all: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(EXE)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

driver.o: driver.cpp driver.h
	$(CC) $(CFLAGS) driver.cpp

model.o: model.cpp model.h
	$(CC) $(CFLAGS) model.cpp

clean:
	rm -rf $(OBJS) $(EXE)
