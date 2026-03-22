#!/usr/bin/env bash
set -euo pipefail

project_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)

cd "$project_root"

if [[ ! -x "./bin/tester_point7_c" ]]; then
    echo "Executable ./bin/tester_point7_c not found." >&2
    echo "Run make first." >&2
    exit 1
fi

if [[ ! -x "./bin/tester_point7_final" ]]; then
    echo "Executable ./bin/tester_point7_final not found." >&2
    echo "Run make first." >&2
    exit 1
fi

if [[ ! -d "./tests/big_tests" ]]; then
    echo "Directory ./tests/big_tests not found." >&2
    echo "Generate tests for point 0 first." >&2
    exit 1
fi

mkdir -p ./results ./plots

./bin/tester_point7_c
./bin/tester_point7_final

python3 ./scripts/plot_point7_c.py
python3 ./scripts/plot_point7_final.py

echo "Point 7 testing finished successfully."

