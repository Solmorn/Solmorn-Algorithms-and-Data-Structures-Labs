#!/usr/bin/env bash
set -euo pipefail

make point4
./bin/tester_point4
python3 ./scripts/plot_point4.py
