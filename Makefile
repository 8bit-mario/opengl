CPP=g++
LIBS=-lGL -lglfw
SOURCES=glad.c main.c
INC_DIR=glad/include

.PHONY: default clean

default: out/main

out/main: out/main.o out/glad.o
	$(CPP) -o $@ $^ $(LIBS)

out/main.o: main.cpp | out
	$(CPP) -o $@ -c $< -I$(INC_DIR)

out/glad.o: glad.c | out
	$(CPP) -o $@ -c $< -I$(INC_DIR)

out:
	mkdir -p $@

clean:
	rm out/*
