#!/usr/bin/env bash
set -euo pipefail

mkdir -p tests/point1

SEED=42
MAX_VALUE=1000000000

for n in 100000 200000 300000 400000 500000 600000 700000 800000 900000 1000000
do
    ./bin/generator "$n" "$MAX_VALUE" "$SEED" > "./tests/point1/test_${n}.txt"
    echo "generated test_${n}.txt"
done