#!/usr/bin/env bash
set -euo pipefail

make point3
./bin/tester_point3
python3 ./scripts/plot_point3.py
