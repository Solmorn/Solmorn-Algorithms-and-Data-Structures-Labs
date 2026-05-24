#!/usr/bin/env bash
set -euo pipefail

make point1
./scripts/generate_point1_tests.sh
./bin/tester_point1
python3 ./scripts/plot_point1.py

echo "point 1 completed"
echo "results: ./results/results_point1_*"
echo "plots:   ./plots/point1_*.svg"
