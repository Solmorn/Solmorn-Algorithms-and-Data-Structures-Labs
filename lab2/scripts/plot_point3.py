import csv
from pathlib import Path
import matplotlib.pyplot as plt


def read_csv(path):
    xs = []
    ys = []

    with open(path, "r", encoding="utf-8") as file:
        reader = csv.DictReader(file)
        for row in reader:
            xs.append(int(row["size"]))
            ys.append(float(row["avg_time_sec"]))

    return xs, ys


def main():
    recursive_x, recursive_y = read_csv("results/results_merge_recursive.csv")
    iterative_x, iterative_y = read_csv("results/results_merge_iterative.csv")

    Path("plots").mkdir(exist_ok=True)

    plt.figure(figsize=(10, 6))

    plt.plot(recursive_x, recursive_y, label="Recursive merge sort")
    plt.plot(iterative_x, iterative_y, label="Iterative merge sort")

    plt.xlabel("Array size")
    plt.ylabel("Average time, sec")
    plt.title("Recursive vs iterative merge sort on big_tests")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/plot_point3.svg", format="svg")
    plt.close()


if __name__ == "__main__":
    main()