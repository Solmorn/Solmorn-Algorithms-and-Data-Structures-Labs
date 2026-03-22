#!/usr/bin/env bash
set -euo pipefail

project_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)

cd "$project_root"

if [[ ! -x "./bin/tester_point1" ]]; then
    echo "Executable ./bin/tester_point1 not found." >&2
    echo "Run make first." >&2
    exit 1
fi

if [[ ! -d "./tests/small_tests" ]]; then
    echo "Directory ./tests/small_tests not found." >&2
    echo "Generate tests for point 0 first." >&2
    exit 1
fi

mkdir -p ./results ./plots

./bin/tester_point1
python3 ./scripts/plot_point1.py

echo "Point 1 testing finished successfully."

