CC = g++
CFLAGS = -O3
INCLUDE_DIR = include/
_SRC = branch_and_bound.cpp graph.cpp graph_utils.cpp
SRC_DIR = src
SRC = $(patsubst %,$(SRC_DIR)/%,$(_SRC))
HEADERS = branch_and_bound.hpp graph.hpp graph_utils.hpp
_OBJ = main.o branch_and_bound.o graph.o graph_utils.o
OBJ_DIR = obj
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c -o $@ $< -I $(INCLUDE_DIR) $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $(OBJ) -I $(INCLUDE_DIR) $(CFLAGS)

clean:
	rm -f obj/* main
