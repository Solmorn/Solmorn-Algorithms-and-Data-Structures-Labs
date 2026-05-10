#!/usr/bin/env bash
set -euo pipefail

make point1
./bin/tester_point1
python3 ./scripts/plot_point1.py
