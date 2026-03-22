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
    Path("plots").mkdir(exist_ok=True)

    plt.figure(figsize=(10, 6))

    cutoffs = [8, 16, 24, 32, 40, 48, 64]

    for t in cutoffs:
        x, y = read_csv(f"results/results_shell_cutoff_t{t}.csv")
        plt.plot(x, y, label=f"t = {t}")

    plt.xlabel("Array size")
    plt.ylabel("Average time, sec")
    plt.title("Quick sort + Shell sort cutoff on big_tests")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/plot_point6.svg", format="svg")
    plt.close()


if __name__ == "__main__":
    main()

    