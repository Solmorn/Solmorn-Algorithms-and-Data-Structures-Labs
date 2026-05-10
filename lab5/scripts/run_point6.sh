#!/usr/bin/env bash
set -euo pipefail

make point6
./bin/tester_point6
python3 ./scripts/plot_point6.py
