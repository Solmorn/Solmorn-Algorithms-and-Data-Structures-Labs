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
    lomuto_x, lomuto_y = read_csv("results/results_quick_lomuto_duplicates.csv")
    hoare_x, hoare_y = read_csv("results/results_quick_hoare_duplicates.csv")
    fat_x, fat_y = read_csv("results/results_quick_fat_duplicates.csv")

    Path("plots").mkdir(exist_ok=True)

    plt.figure(figsize=(10, 6))

    plt.plot(lomuto_x, lomuto_y, label="Quick sort (Lomuto)")
    plt.plot(hoare_x, hoare_y, label="Quick sort (Hoare)")
    plt.plot(fat_x, fat_y, label="Quick sort (fat partition)")

    plt.xlabel("Array size")
    plt.ylabel("Average time, sec")
    plt.title("Quick sort partition schemes on duplicate-heavy tests")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/plot_point4_duplicates.svg", format="svg")
    plt.close()


if __name__ == "__main__":
    main()

    