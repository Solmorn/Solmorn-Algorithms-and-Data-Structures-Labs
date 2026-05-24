#!/usr/bin/env bash
set -euo pipefail

project_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)

cd "$project_root"

if [[ ! -x "./bin/tester_point4_partitions" ]]; then
    echo "Executable ./bin/tester_point4_partitions not found." >&2
    echo "Run make first." >&2
    exit 1
fi

if [[ ! -x "./bin/tester_point4_optimizations" ]]; then
    echo "Executable ./bin/tester_point4_optimizations not found." >&2
    echo "Run make first." >&2
    exit 1
fi

if [[ ! -d "./tests/big_tests" ]]; then
    echo "Directory ./tests/big_tests not found." >&2
    echo "Generate tests for point 0 first." >&2
    exit 1
fi

if [[ ! -d "./tests/test_most_dublicates" ]]; then
    echo "Directory ./tests/test_most_dublicates not found." >&2
    echo "Generate tests for point 0 first." >&2
    exit 1
fi

mkdir -p ./results ./plots

./bin/tester_point4_partitions
./bin/tester_point4_optimizations

python3 ./scripts/plot_point4_big.py
python3 ./scripts/plot_point4_duplicates.py
python3 ./scripts/plot_point4_optimizations.py

echo "Point 4 testing finished successfully."