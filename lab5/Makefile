CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wpedantic
BIN_DIR = bin
SRC_DIR = src
INC_DIR = include

COMMON_SRC = $(SRC_DIR)/tree_testing.c

all: point1 point2 point3 point4 point5 point6 point7

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

dirs:
	mkdir -p results plots $(BIN_DIR)

$(BIN_DIR)/tester_point1: $(SRC_DIR)/tester_point1.c $(COMMON_SRC) $(SRC_DIR)/naive_tree.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $^ -o $@

$(BIN_DIR)/tester_point2: $(SRC_DIR)/tester_point2.c $(COMMON_SRC) $(SRC_DIR)/avl_tree.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $^ -o $@

$(BIN_DIR)/tester_point3: $(SRC_DIR)/tester_point3.c $(COMMON_SRC) $(SRC_DIR)/treap.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $^ -o $@

$(BIN_DIR)/tester_point4: $(SRC_DIR)/tester_point4.c $(COMMON_SRC) $(SRC_DIR)/splay_tree.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $^ -o $@

$(BIN_DIR)/tester_point5: $(SRC_DIR)/tester_point5.c $(COMMON_SRC) $(SRC_DIR)/rb_tree.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $^ -o $@

$(BIN_DIR)/tester_point6: $(SRC_DIR)/tester_point6.c $(COMMON_SRC) $(SRC_DIR)/b_tree.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $^ -o $@

$(BIN_DIR)/tester_point7: $(SRC_DIR)/tester_point7.c $(COMMON_SRC) $(SRC_DIR)/skip_list.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $^ -o $@

point1: dirs $(BIN_DIR)/tester_point1
point2: dirs $(BIN_DIR)/tester_point2
point3: dirs $(BIN_DIR)/tester_point3
point4: dirs $(BIN_DIR)/tester_point4
point5: dirs $(BIN_DIR)/tester_point5
point6: dirs $(BIN_DIR)/tester_point6
point7: dirs $(BIN_DIR)/tester_point7

clean:
	rm -rf $(BIN_DIR) results/*.csv plots/*.svg

.PHONY: all point1 point2 point3 point4 point5 point6 point7 clean dirs
