#!/usr/bin/env bash
set -euo pipefail

script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
project_root=$(cd "$script_dir/.." && pwd)

generator_bin="$project_root/bin/generator"
qsort_bin="$project_root/bin/qsort_stream"
generate_script="$project_root/scripts/generate_tests.sh"

MAX_RAND=2147483647

"$generate_script" 0 1000 50 "$MAX_RAND" 5 \
    "$project_root/tests/small_tests" \
    "$generator_bin" "$qsort_bin"

"$generate_script" 0 1000000 10000 "$MAX_RAND" 5 \
    "$project_root/tests/big_tests" \
    "$generator_bin" "$qsort_bin"

"$generate_script" 0 1000000 10000 10000 5 \
    "$project_root/tests/test_most_dublicates" \
    "$generator_bin" "$qsort_bin"



