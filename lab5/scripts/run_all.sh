#!/usr/bin/env bash
set -euo pipefail

if python3 -c "import matplotlib" >/dev/null 2>&1; then
    :
else
    if [ ! -f ".venv/bin/activate" ]; then
        python3 -m venv .venv
    fi
    source .venv/bin/activate
    if ! python3 -c "import matplotlib" >/dev/null 2>&1; then
        python3 -m pip install matplotlib
    fi
fi

./scripts/run_point1.sh
./scripts/run_point2.sh
./scripts/run_point3.sh
./scripts/run_point4.sh
./scripts/run_point5.sh
./scripts/run_point6.sh
./scripts/run_point7.sh
