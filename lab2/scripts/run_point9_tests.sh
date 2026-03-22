#!/usr/bin/env bash
set -euo pipefail

project_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)

cd "$project_root"

if [[ ! -x "./bin/tester_point9" ]]; then
    echo "Executable ./bin/tester_point9 not found." >&2
    echo "Run make first." >&2
    exit 1
fi

if [[ ! -d "./tests/big_tests" ]]; then
    echo "Directory ./tests/big_tests not found." >&2
    echo "Generate tests for point 0 first." >&2
    exit 1
fi

mkdir -p ./results ./plots

./bin/tester_point9
python3 ./scripts/plot_point9.py

echo "Point 9 testing finished successfully."