#!/usr/bin/env bash
set -euo pipefail

mkdir -p ./tests/point1

./bin/generator_point1 uint   1000000 42 ./tests/point1/uint_keys.txt
./bin/generator_point1 float  1000000 42 ./tests/point1/float_keys.txt
./bin/generator_point1 string 1000000 42 ./tests/point1/string_keys.txt

echo "point 1 test data generated"
echo "uints:   ./tests/point1/uint_keys.txt"
echo "floats:  ./tests/point1/float_keys.txt"
echo "strings: ./tests/point1/string_keys.txt"
