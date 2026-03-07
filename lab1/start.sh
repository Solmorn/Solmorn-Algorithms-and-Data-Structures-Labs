#!/bin/bash

set -e

echo "Starting testing"

gcc main.c tests.c arr_stack.c list_stack.c -o testing

./testing

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

echo "Done"