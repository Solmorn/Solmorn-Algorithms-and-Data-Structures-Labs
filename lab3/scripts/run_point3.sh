#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

mkdir -p tests/point3 results plots bin

make point3

./scripts/generate_point3_tests.sh
./bin/tester_point3
python3 ./scripts/plot_point3.py

echo "point 3 completed"
echo "results: ./results/results_point3_sparse.csv"
echo "         ./results/results_point3_dense.csv"
echo "plots:   ./plots/plot_point3_sparse.svg"
echo "         ./plots/plot_point3_dense.svg"
