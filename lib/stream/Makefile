CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

OUT := ../
INCLUDE := ../

all: $(OUT)/libstream.a

stream.o: stream.c stream.h
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c stream.c

$(OUT)/libstream.a: stream.o
	ar rcs $@ stream.o

