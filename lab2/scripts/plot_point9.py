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
    lsd_x, lsd_y = read_csv("results/results_lsd_radix.csv")
    msd_x, msd_y = read_csv("results/results_msd_radix.csv")

    Path("plots").mkdir(exist_ok=True)

    plt.figure(figsize=(10, 6))

    plt.plot(lsd_x, lsd_y, label="LSD radix sort")
    plt.plot(msd_x, msd_y, label="MSD radix sort")

    plt.xlabel("Array size")
    plt.ylabel("Average time, sec")
    plt.title("LSD vs MSD radix sort on big_tests")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/plot_point9.svg", format="svg")
    plt.close()


if __name__ == "__main__":
    main()

    