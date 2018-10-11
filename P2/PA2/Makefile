CC=g++
CFLAGS=-Wall -Wextra -c # -pedantic   COMMENTED OUT FOR GLM UNNAMED STRUCT WARNINGS -- TODO FIX THIS
LFLAGS=-I.
OBJS=main.o driver.o model.o wavefront_obj.o camera.o light_source.o material.o
OUTPUT_DIRS=driver0*/
EXE=raytracer

all: $(OBJS)

	$(CC) $(LFLAGS) $(OBJS) -o $(EXE)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

camera.o: camera.h camera.cpp
	$(CC) $(CFLAGS) camera.cpp

driver.o: driver.cpp driver.h
	$(CC) $(CFLAGS) driver.cpp

light_source.o: light_source.cpp light_source.h
	$(CC) $(CFLAGS) light_source.cpp

material.o: material.cpp material.h
	$(CC) $(CFLAGS) material.cpp

model.o: model.cpp model.h
	$(CC) $(CFLAGS) model.cpp

wavefront_obj.o: wavefront_obj.cpp wavefront_obj.h
	$(CC) $(CFLAGS) wavefront_obj.cpp

clean:
	rm -rf $(OBJS) $(EXE) $(OUTPUT_DIRS)
