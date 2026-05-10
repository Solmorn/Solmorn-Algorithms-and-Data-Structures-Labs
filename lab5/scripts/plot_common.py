#!/usr/bin/env python3

import csv
import sys
from pathlib import Path

import matplotlib.pyplot as plt


def read_common_csv(path):
    ns = []
    insert_times = []
    erase_times = []

    with open(path, newline="") as file:
        reader = csv.DictReader(file)
        for row in reader:
            ns.append(int(row["n"]))
            insert_times.append(float(row["insert_time"]))
            erase_times.append(float(row["erase_time"]))

    return ns, insert_times, erase_times


def plot_common(csv_path, output_path, title):
    ns, insert_times, erase_times = read_common_csv(csv_path)

    plt.figure(figsize=(9, 5))
    plt.plot(ns, insert_times, marker="o", label="insert")
    plt.plot(ns, erase_times, marker="o", label="erase")
    plt.xlabel("n")
    plt.ylabel("time, seconds")
    plt.title(title)
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(output_path)
    plt.close()


def main():
    if len(sys.argv) != 4:
        print("usage: plot_common.py <input.csv> <output.svg> <title>", file=sys.stderr)
        return 1

    csv_path = Path(sys.argv[1])
    output_path = Path(sys.argv[2])
    title = sys.argv[3]

    output_path.parent.mkdir(parents=True, exist_ok=True)
    plot_common(csv_path, output_path, title)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
