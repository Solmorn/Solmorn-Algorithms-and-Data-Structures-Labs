CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wpedantic

ROOT_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
BIN_DIR = $(ROOT_DIR)/bin
SRC_DIR = $(ROOT_DIR)/src
INC_DIR = $(ROOT_DIR)/include
TEST_DIR = $(ROOT_DIR)/tests
RESULTS_DIR = $(ROOT_DIR)/results
PLOTS_DIR = $(ROOT_DIR)/plots
SCRIPTS_DIR = $(ROOT_DIR)/scripts
VENV_PYTHON = $(ROOT_DIR)/.venv/bin/python3

all: point1 point2 point3

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

dirs:
	mkdir -p $(TEST_DIR)/point1 $(TEST_DIR)/point2 $(TEST_DIR)/point3 $(RESULTS_DIR) $(PLOTS_DIR) $(BIN_DIR)

python-env:
	cd "$(ROOT_DIR)" && \
	if [ ! -x ".venv/bin/python3" ]; then python3 -m venv .venv; fi && \
	if ! .venv/bin/python3 -c "import matplotlib" >/dev/null 2>&1; then \
		.venv/bin/python3 -m pip install --upgrade pip; \
		.venv/bin/python3 -m pip install matplotlib; \
	fi

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

generate-point1: point1
	cd "$(ROOT_DIR)" && \
	SEED=42; MAX_VALUE=1000000000; \
	for n in 100000 200000 300000 400000 500000 600000 700000 800000 900000 1000000; do \
		"$(BIN_DIR)/generator" "$$n" "$$MAX_VALUE" "$$SEED" > "$(TEST_DIR)/point1/test_$${n}.txt"; \
		echo "generated test_$${n}.txt"; \
	done

generate-point2: point2
	cd "$(ROOT_DIR)" && \
	SEED=42; MAX_VALUE=1000000000; \
	for n in 100000 200000 300000 400000 500000 600000 700000 800000 900000 1000000; do \
		"$(BIN_DIR)/generator" "$$n" "$$MAX_VALUE" "$$SEED" > "$(TEST_DIR)/point2/test_$${n}.txt"; \
		echo "generated test_$${n}.txt"; \
	done

generate-point3: point3
	cd "$(ROOT_DIR)" && \
	SEED=42; MAX_WEIGHT=1000000; \
	for n in 1000 2000 3000 4000 5000; do \
		"$(BIN_DIR)/generator_point3" sparse "$$n" "$$MAX_WEIGHT" "$$SEED" > "$(TEST_DIR)/point3/sparse_$${n}.bin"; \
		echo "generated sparse_$${n}.bin"; \
		"$(BIN_DIR)/generator_point3" dense "$$n" "$$MAX_WEIGHT" "$$SEED" > "$(TEST_DIR)/point3/dense_$${n}.bin"; \
		echo "generated dense_$${n}.bin"; \
	done

run-point1: python-env generate-point1
	cd "$(ROOT_DIR)" && "$(BIN_DIR)/tester_point1" && "$(VENV_PYTHON)" "$(SCRIPTS_DIR)/plot_point1.py"
	@echo "point 1 completed"
	@echo "results: $(RESULTS_DIR)/results_point1.csv"
	@echo "plot:    $(PLOTS_DIR)/plot_point1.svg"

run-point2: python-env generate-point2
	cd "$(ROOT_DIR)" && "$(BIN_DIR)/tester_point2" && "$(VENV_PYTHON)" "$(SCRIPTS_DIR)/plot_point2.py"
	@echo "point 2 completed"
	@echo "results: $(RESULTS_DIR)/results_point2.csv"
	@echo "plots:   $(PLOTS_DIR)/plot_point2.svg"
	@echo "         $(PLOTS_DIR)/plot_point2_normalized.svg"

run-point3: python-env generate-point3
	cd "$(ROOT_DIR)" && "$(BIN_DIR)/tester_point3" && "$(VENV_PYTHON)" "$(SCRIPTS_DIR)/plot_point3.py"
	@echo "point 3 completed"
	@echo "results: $(RESULTS_DIR)/results_point3_sparse.csv"
	@echo "         $(RESULTS_DIR)/results_point3_dense.csv"
	@echo "plots:   $(PLOTS_DIR)/plot_point3_sparse.svg"
	@echo "         $(PLOTS_DIR)/plot_point3_dense.svg"

run-all: run-point1 run-point2 run-point3

clean:
	rm -rf $(BIN_DIR) $(RESULTS_DIR)/*.csv $(PLOTS_DIR)/*.svg

.PHONY: all point1 point2 point3 generate-point1 generate-point2 generate-point3 run-point1 run-point2 run-point3 run-all clean dirs python-env
