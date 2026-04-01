CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wpedantic

BIN_DIR = bin
SRC_DIR = src
INC_DIR = include

all: point1 point2 point3

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

dirs:
	mkdir -p tests/point1 tests/point2 tests/point3 results plots $(BIN_DIR)

$(BIN_DIR)/generator: $(SRC_DIR)/generator.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $< -o $@

$(BIN_DIR)/tester_point1: $(SRC_DIR)/tester_point1.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $< -o $@

$(BIN_DIR)/tester_point2: $(SRC_DIR)/tester_point2.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $< -o $@

$(BIN_DIR)/generator_point3: $(SRC_DIR)/generator_point3.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $< -o $@

$(BIN_DIR)/tester_point3: \
	$(SRC_DIR)/tester_point3.c \
	$(SRC_DIR)/dijkstra_naive.c \
	$(SRC_DIR)/dijkstra_binary.c \
	$(SRC_DIR)/dijkstra_binomial.c \
	$(SRC_DIR)/dijkstra_fibonacci.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) $^ -o $@

point1: dirs $(BIN_DIR)/generator $(BIN_DIR)/tester_point1
point2: dirs $(BIN_DIR)/generator $(BIN_DIR)/tester_point2
point3: dirs $(BIN_DIR)/generator_point3 $(BIN_DIR)/tester_point3

clean:
	rm -rf $(BIN_DIR) results/*.csv plots/*.svg

.PHONY: all point1 point2 point3 clean dirs