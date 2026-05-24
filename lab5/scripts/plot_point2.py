#!/usr/bin/env python3

from plot_common import plot_common


def main():
    plot_common("results/results_point2_random.csv", "plots/point2_random.svg", "Point 2. AVL tree, random input")
    plot_common("results/results_point2_sorted.csv", "plots/point2_sorted.svg", "Point 2. AVL tree, sorted input")


if __name__ == "__main__":
    main()
