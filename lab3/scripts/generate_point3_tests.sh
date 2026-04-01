#!/usr/bin/env bash
set -euo pipefail

mkdir -p tests/point3

SEED=42
MAX_WEIGHT=1000000

for n in 1000 2000 3000 4000 5000
do
    ./bin/generator_point3 sparse "$n" "$MAX_WEIGHT" "$SEED" > "./tests/point3/sparse_${n}.bin"
    echo "generated sparse_${n}.bin"

    ./bin/generator_point3 dense "$n" "$MAX_WEIGHT" "$SEED" > "./tests/point3/dense_${n}.bin"
    echo "generated dense_${n}.bin"
done
