#!/usr/bin/env bash
set -euo pipefail

make point7
./bin/tester_point7
python3 ./scripts/plot_point7.py
