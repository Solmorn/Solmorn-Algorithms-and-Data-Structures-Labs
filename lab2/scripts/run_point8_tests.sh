#!/usr/bin/env bash
set -euo pipefail

project_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)

cd "$project_root"

if [[ ! -x "./bin/tester_point8" ]]; then
    echo "Executable ./bin/tester_point8 not found." >&2
    echo "Run make first." >&2
    exit 1
fi

if [[ ! -f "./third_party/pdqsort/pdqsort.h" ]]; then
    echo "third_party/pdqsort/pdqsort.h not found." >&2
    echo "Vendor pdqsort from GitHub first." >&2
    exit 1
fi

if [[ ! -f "./third_party/cpp-TimSort/include/gfx/timsort.hpp" ]]; then
    echo "third_party/cpp-TimSort/include/gfx/timsort.hpp not found." >&2
    echo "Vendor TimSort from GitHub first." >&2
    exit 1
fi

if [[ ! -d "./tests/big_tests" ]]; then
    echo "Directory ./tests/big_tests not found." >&2
    echo "Generate tests for point 0 first." >&2
    exit 1
fi

mkdir -p ./results ./plots

./bin/tester_point8
python3 ./scripts/plot_point8.py

echo "Point 8 testing finished successfully."

