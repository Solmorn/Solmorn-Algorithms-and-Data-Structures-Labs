#!/usr/bin/env bash
set -euo pipefail

mkdir -p ./results ./plots ./bin

make point3
./bin/tester_point3
python3 ./scripts/plot_point3.py

echo "point 3 completed"
echo "results: ./results/results_point3_summary.csv"
echo "plots:   ./plots/point3_*.svg"

