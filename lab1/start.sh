#!/bin/bash
set -e

echo "Starting testing"

gcc -Wall -Wextra -Wpedantic -Iheaders src/main.c src/tests.c src/arr_stack.c src/list_stack.c -o testing
./testing

if [ -f "make_graph.py" ]; then
    if [ ! -d ".venv" ]; then
        echo "Creating virtual environment"
        python3 -m venv .venv
    fi

    source .venv/bin/activate

    if [ ! -f ".venv/.deps_installed" ]; then
        echo "Installing Python libraries"
        python -m pip install --upgrade pip
        python -m pip install pandas matplotlib
        touch .venv/.deps_installed
    fi

    python make_graph.py
fi

echo "Done"
