#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")/.."

run_step() {
    echo
    echo "==> $1"
    shift
    "$@"
}

POINT8_READY=0
if [[ -f ./third_party/pdqsort/pdqsort.h && -f ./third_party/cpp-TimSort/include/gfx/timsort.hpp ]]; then
    POINT8_READY=1
fi

rm -rf ./results ./plots
mkdir -p ./results ./plots

run_step "Generating tests" ./scripts/generate_required_tests.sh

if [[ $POINT8_READY -eq 1 ]]; then
    run_step "Building all targets" make
else
    echo
    echo "==> Point 8 dependencies were not found, so point 8 and point 10 will be skipped."
    run_step "Building targets except point 8" make \
        bin/generator \
        bin/qsort_stream \
        bin/tester \
        bin/tester_point1 \
        bin/tester_point2 \
        bin/tester_point3 \
        bin/tester_point4_partitions \
        bin/tester_point4_optimizations \
        bin/tester_point5 \
        bin/tester_point6 \
        bin/tester_point7_c \
        bin/tester_point7_final \
        bin/tester_point9
fi

run_step "Running point 0 smoke test" \
    ./bin/tester ./tests/small_tests ./results/results_qsort_small.csv 0 1000 50

run_step "Running point 1" ./scripts/run_point1_tests.sh
run_step "Running point 2" ./scripts/run_point2_tests.sh
run_step "Running point 3" ./scripts/run_point3_tests.sh
run_step "Running point 4" ./scripts/run_point4_tests.sh
run_step "Running point 5" ./scripts/run_point5_tests.sh
run_step "Running point 6" ./scripts/run_point6_tests.sh
run_step "Running point 7" ./scripts/run_point7_tests.sh

if [[ $POINT8_READY -eq 1 ]]; then
    run_step "Running point 8" ./scripts/run_point8_tests.sh
else
    echo
    echo "==> Skipping point 8"
fi

run_step "Running point 9" ./scripts/run_point9_tests.sh

if [[ $POINT8_READY -eq 1 ]]; then
    run_step "Running point 10" ./scripts/run_point10_tests.sh
else
    echo
    echo "==> Skipping point 10 because it uses point 8 results on the final graph"
fi

echo
echo "All available tests finished successfully."