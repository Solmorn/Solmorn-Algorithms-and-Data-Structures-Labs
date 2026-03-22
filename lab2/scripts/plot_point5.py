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
    middle_x, middle_y = read_csv("results/results_pivot_middle.csv")
    median3_x, median3_y = read_csv("results/results_pivot_median3.csv")
    random_x, random_y = read_csv("results/results_pivot_random.csv")
    median3_random_x, median3_random_y = read_csv("results/results_pivot_median3_random.csv")
    mom_x, mom_y = read_csv("results/results_pivot_median_of_medians.csv")

    Path("plots").mkdir(exist_ok=True)

    plt.figure(figsize=(10, 6))

    plt.plot(middle_x, middle_y, label="Pivot: middle")
    plt.plot(median3_x, median3_y, label="Pivot: median of 3")
    plt.plot(random_x, random_y, label="Pivot: random")
    plt.plot(median3_random_x, median3_random_y, label="Pivot: median of 3 random")
    plt.plot(mom_x, mom_y, label="Pivot: median of medians")

    plt.xlabel("Array size")
    plt.ylabel("Average time, sec")
    plt.title("Pivot strategy comparison on big_tests")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/plot_point5.svg", format="svg")
    plt.close()


if __name__ == "__main__":
    main()