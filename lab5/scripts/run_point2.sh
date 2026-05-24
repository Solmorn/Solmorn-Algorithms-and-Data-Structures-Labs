#!/usr/bin/env bash
set -euo pipefail

make point2
./bin/tester_point2
python3 ./scripts/plot_point2.py
