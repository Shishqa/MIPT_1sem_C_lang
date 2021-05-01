CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

OUT := ../
INCLUDE := ../

all: $(OUT)/libstack.a

stack.o: stack.cpp stack.h
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c stack.cpp

$(OUT)/libstack.a: stack.o
	ar rcs $@ stack.o

