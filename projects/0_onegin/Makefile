CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

INCLUDE := ../../lib/:../../lib/stream/
LIBRARIES := ../../lib

all: onegin

onegin: src/main.c
	$(CXX) $(CXX_FLAGS) -I../../lib/ -I../../lib/stream/ -L$(LIBRARIES) $^ -o $@
