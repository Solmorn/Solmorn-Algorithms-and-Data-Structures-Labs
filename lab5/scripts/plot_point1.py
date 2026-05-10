#!/usr/bin/env python3

import csv
from pathlib import Path

import matplotlib.pyplot as plt


def main():
    labels = []
    insert_times = []
    erase_times = []

    with open("results/results_point1.csv", newline="") as file:
        reader = csv.DictReader(file)
        for row in reader:
            labels.append(f"{row['type']}\nn={row['n']}")
            insert_times.append(float(row["insert_time"]))
            erase_times.append(float(row["erase_time"]))

    x_positions = list(range(len(labels)))
    width = 0.35

    plt.figure(figsize=(8, 5))
    plt.bar([x - width / 2 for x in x_positions], insert_times, width=width, label="insert")
    plt.bar([x + width / 2 for x in x_positions], erase_times, width=width, label="erase")
    plt.xticks(x_positions, labels)
    plt.ylabel("time, seconds")
    plt.title("Point 1. Naive BST")
    plt.grid(True, axis="y")
    plt.legend()
    plt.tight_layout()

    Path("plots").mkdir(parents=True, exist_ok=True)
    plt.savefig("plots/point1.svg")
    plt.close()


if __name__ == "__main__":
    main()
