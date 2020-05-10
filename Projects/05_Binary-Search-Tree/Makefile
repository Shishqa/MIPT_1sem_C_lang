CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb 

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= ../../Include/Lib

LIBRARIES	:= -lMSLib
EXECUTABLE	:= run_tests


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
