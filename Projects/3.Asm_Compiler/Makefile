CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN		:= bin
SRC		:= src

SUBPROJ1	:= asm
SUBPROJ2	:= cpu
SUBPROJ3	:= disasm
SUBPROJ4	:= make_asm

INCLUDE	:= ../../Include
LIB		:= ../../Include/Lib

LIBRARIES	:= MSLib
EXECUTABLE	:= make_asm


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*

	CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp hello.cpp factorial.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hello

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

asm: asm.o
	