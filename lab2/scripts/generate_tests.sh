#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 8 ]]; then
    echo "Usage: $0 FROM TO STEP LIMIT TESTS_PER_SIZE OUT_DIR GENERATOR_BIN QSORT_BIN" >&2
    exit 1
fi

from=$1
to=$2
step=$3
limit=$4
tests_per_size=$5
out_dir=$6
generator_bin=$7
qsort_bin=$8

if (( step <= 0 )); then
    echo "STEP must be positive" >&2
    exit 1
fi

if (( to < from )); then
    echo "TO must be >= FROM" >&2
    exit 1
fi

if (( tests_per_size <= 0 )); then
    echo "TESTS_PER_SIZE must be positive" >&2
    exit 1
fi

mkdir -p "$out_dir"

for (( size=from; size<=to; size+=step )); do
    for (( k=0; k<tests_per_size; ++k )); do
        input_file="$out_dir/${size}_${k}.in"
        output_file="$out_dir/${size}_${k}.out"

        "$generator_bin" "$size" "$limit" > "$input_file"
        "$qsort_bin" < "$input_file" > "$output_file"
    done
done


