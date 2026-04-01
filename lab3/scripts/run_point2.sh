#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

mkdir -p tests/point2 results plots bin

make point2

./scripts/generate_point2_tests.sh
./bin/tester_point2
python3 ./scripts/plot_point2.py

echo "point 2 completed"
echo "results: ./results/results_point2.csv"
echo "plots:   ./plots/plot_point2.svg"
echo "         ./plots/plot_point2_normalized.svg"