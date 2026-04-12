#!/usr/bin/env bash
set -euo pipefail

mkdir -p ./results ./plots ./bin

make point2
./bin/tester_point2
python3 ./scripts/plot_point2.py

echo "point 2 completed"
echo "results: ./results/results_point2_*"
echo "plots:   ./plots/point2_*.svg"