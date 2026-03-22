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
    quick_x, quick_y = read_csv("results/results_quick_best_cutoff32.csv")
    intro_x, intro_y = read_csv("results/results_introsort_best.csv")

    Path("plots").mkdir(exist_ok=True)

    plt.figure(figsize=(10, 6))

    plt.plot(quick_x, quick_y, label="Best quick sort (median3 + fat + 1 branch + cutoff32)")
    plt.plot(intro_x, intro_y, label="Introsort")

    plt.xlabel("Array size")
    plt.ylabel("Average time, sec")
    plt.title("Best quick sort vs introsort on big_tests")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/plot_point7_final.svg", format="svg")
    plt.close()


if __name__ == "__main__":
    main()