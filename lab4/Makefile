CC := gcc

CFLAGS := -std=c11 -O2 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wno-unused-function -D_POSIX_C_SOURCE=200809L
CPPFLAGS := -I./include
LDFLAGS :=
LDLIBS :=

BIN_DIR := ./bin
SRC_DIR := ./src

POINT1_GENERATOR := $(BIN_DIR)/generator_point1
POINT1_TARGET := $(BIN_DIR)/tester_point1
POINT2_TARGET := $(BIN_DIR)/tester_point2
POINT3_TARGET := $(BIN_DIR)/tester_point3

.PHONY: all point1 point2 point3 clean

all: point1 point2 point3

point1: $(POINT1_GENERATOR) $(POINT1_TARGET)
point2: $(POINT2_TARGET)
point3: $(POINT3_TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(POINT1_GENERATOR): $(SRC_DIR)/generator_point1.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

$(POINT1_TARGET): $(SRC_DIR)/tester_point1.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

$(POINT2_TARGET): $(SRC_DIR)/tester_point2.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

$(POINT3_TARGET): $(SRC_DIR)/tester_point3.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf $(BIN_DIR)