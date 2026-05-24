CC = gcc
CXX = g++

CFLAGS = -std=c11 -O2 -Wall -Wextra -Wpedantic -Iinclude
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra -Wpedantic -Iinclude -Ithird_party/cpp-TimSort/include

BIN_DIR = bin
SRC_DIR = src
INC_DIR = include
SORT_UTILS = $(SRC_DIR)/sort_utils.c

POINT0_BIN = \
	$(BIN_DIR)/generator \
	$(BIN_DIR)/qsort_stream \
	$(BIN_DIR)/tester

POINT1_BIN = \
	$(BIN_DIR)/tester_point1

POINT2_BIN = \
	$(BIN_DIR)/tester_point2

POINT3_BIN = \
	$(BIN_DIR)/tester_point3

POINT4_BIN = \
	$(BIN_DIR)/tester_point4_partitions \
	$(BIN_DIR)/tester_point4_optimizations

POINT5_BIN = \
	$(BIN_DIR)/tester_point5

POINT6_BIN = \
	$(BIN_DIR)/tester_point6

POINT7_BIN = \
	$(BIN_DIR)/tester_point7_c \
	$(BIN_DIR)/tester_point7_final

POINT8_BIN = \
	$(BIN_DIR)/tester_point8

POINT9_BIN = \
	$(BIN_DIR)/tester_point9

POINT10_BIN = \
	$(BIN_DIR)/tester_point10

all: $(BIN_DIR) $(POINT0_BIN) $(POINT1_BIN) $(POINT2_BIN) $(POINT3_BIN) $(POINT4_BIN) $(POINT5_BIN) $(POINT6_BIN) $(POINT7_BIN) $(POINT8_BIN) $(POINT9_BIN) $(POINT10_BIN)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/generator: $(SRC_DIR)/generator.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/qsort_stream: $(SRC_DIR)/qsort_stream.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/tester: $(SRC_DIR)/tester.c $(INC_DIR)/testing.h | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/tester_point1: \
	$(SRC_DIR)/tester_point1.c \
	$(SRC_DIR)/insertion_sort.c \
	$(SRC_DIR)/bubble_sort.c \
	$(SRC_DIR)/selection_sort.c \
	$(SRC_DIR)/shell_sort.c \
	$(SORT_UTILS) \
	$(INC_DIR)/testing.h \
	$(INC_DIR)/point1_sorts.h | $(BIN_DIR)
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/tester_point1.c \
		$(SRC_DIR)/insertion_sort.c \
		$(SRC_DIR)/bubble_sort.c \
		$(SRC_DIR)/selection_sort.c \
		$(SRC_DIR)/shell_sort.c \
		$(SORT_UTILS) \
		-o $@

$(BIN_DIR)/tester_point2: \
	$(SRC_DIR)/tester_point2.c \
	$(SRC_DIR)/k_ary_heap_sort.c \
	$(INC_DIR)/testing.h \
	$(INC_DIR)/point2_sorts.h | $(BIN_DIR)
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/tester_point2.c \
		$(SRC_DIR)/k_ary_heap_sort.c \
		-o $@

$(BIN_DIR)/tester_point3: \
	$(SRC_DIR)/tester_point3.c \
	$(SRC_DIR)/merge_sort_recursive.c \
	$(SRC_DIR)/merge_sort_iterative.c \
	$(INC_DIR)/testing.h \
	$(INC_DIR)/point3_sorts.h | $(BIN_DIR)
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/tester_point3.c \
		$(SRC_DIR)/merge_sort_recursive.c \
		$(SRC_DIR)/merge_sort_iterative.c \
		-o $@

$(BIN_DIR)/tester_point4_partitions: \
	$(SRC_DIR)/tester_point4_partitions.c \
	$(SRC_DIR)/quick_sort_partitions.c \
	$(SORT_UTILS) \
	$(INC_DIR)/testing.h \
	$(INC_DIR)/point4_sorts.h | $(BIN_DIR)
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/tester_point4_partitions.c \
		$(SRC_DIR)/quick_sort_partitions.c \
		$(SORT_UTILS) \
		-o $@

$(BIN_DIR)/tester_point4_optimizations: \
	$(SRC_DIR)/tester_point4_optimizations.c \
	$(SRC_DIR)/quick_sort_partitions.c \
	$(SORT_UTILS) \
	$(INC_DIR)/testing.h \
	$(INC_DIR)/point4_sorts.h | $(BIN_DIR)
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/tester_point4_optimizations.c \
		$(SRC_DIR)/quick_sort_partitions.c \
		$(SORT_UTILS) \
		-o $@

$(BIN_DIR)/tester_point5: \
	$(SRC_DIR)/tester_point5.c \
	$(SRC_DIR)/quick_sort_pivot_strategies.c \
	$(SORT_UTILS) \
	$(INC_DIR)/testing.h \
	$(INC_DIR)/point5_sorts.h | $(BIN_DIR)
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/tester_point5.c \
		$(SRC_DIR)/quick_sort_pivot_strategies.c \
		$(SORT_UTILS) \
		-o $@

$(BIN_DIR)/tester_point6: \
	$(SRC_DIR)/tester_point6.c \
	$(SRC_DIR)/quick_sort_shell_cutoff.c \
	$(SORT_UTILS) \
	$(INC_DIR)/testing.h \
	$(INC_DIR)/point6_sorts.h | $(BIN_DIR)
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/tester_point6.c \
		$(SRC_DIR)/quick_sort_shell_cutoff.c \
		$(SORT_UTILS) \
		-o $@

$(BIN_DIR)/tester_point7_c: \
	$(SRC_DIR)/tester_point7_c.c \
	$(SRC_DIR)/introsort.c \
	$(SORT_UTILS) \
	$(INC_DIR)/testing.h \
	$(INC_DIR)/point7_sorts.h | $(BIN_DIR)
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/tester_point7_c.c \
		$(SRC_DIR)/introsort.c \
		$(SORT_UTILS) \
		-o $@

$(BIN_DIR)/tester_point7_final: \
	$(SRC_DIR)/tester_point7_final.c \
	$(SRC_DIR)/introsort.c \
	$(SORT_UTILS) \
	$(INC_DIR)/testing.h \
	$(INC_DIR)/point7_sorts.h | $(BIN_DIR)
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/tester_point7_final.c \
		$(SRC_DIR)/introsort.c \
		$(SORT_UTILS) \
		-o $@

$(BIN_DIR)/tester_point8: \
	$(SRC_DIR)/tester_point8.cpp \
	$(SRC_DIR)/point8_wrappers.cpp \
	$(INC_DIR)/testing.h \
	$(INC_DIR)/point8_sorts.h \
	third_party/pdqsort/pdqsort.h \
	third_party/cpp-TimSort/include/gfx/timsort.hpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) \
		$(SRC_DIR)/tester_point8.cpp \
		$(SRC_DIR)/point8_wrappers.cpp \
		-o $@

$(BIN_DIR)/tester_point9: \
	$(SRC_DIR)/tester_point9.c \
	$(SRC_DIR)/radix_sorts.c \
	$(INC_DIR)/testing.h \
	$(INC_DIR)/point9_sorts.h | $(BIN_DIR)
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/tester_point9.c \
		$(SRC_DIR)/radix_sorts.c \
		-o $@

$(BIN_DIR)/tester_point10: \
	$(SRC_DIR)/tester_point10.c \
	$(INC_DIR)/testing.h | $(BIN_DIR)
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/tester_point10.c \
		-o $@

clean:
	rm -rf $(BIN_DIR)

rebuild: clean all

.PHONY: all clean rebuild
