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
    basic_x, basic_y = read_csv("results/results_quick_fat_basic.csv")
    one_branch_x, one_branch_y = read_csv("results/results_quick_fat_one_branch.csv")
    cutoff_x, cutoff_y = read_csv("results/results_quick_fat_one_branch_cutoff40.csv")

    Path("plots").mkdir(exist_ok=True)

    plt.figure(figsize=(10, 6))

    plt.plot(basic_x, basic_y, label="Fat quick sort")
    plt.plot(one_branch_x, one_branch_y, label="Fat quick sort + one branch recursion")
    plt.plot(cutoff_x, cutoff_y, label="Fat quick sort + one branch + cutoff40")

    plt.xlabel("Array size")
    plt.ylabel("Average time, sec")
    plt.title("Quick sort optimizations on big_tests")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/plot_point4_optimizations.svg", format="svg")
    plt.close()


if __name__ == "__main__":
    main()

    