#!/usr/bin/env bash
set -euo pipefail

make point5
./bin/tester_point5
python3 ./scripts/plot_point5.py
