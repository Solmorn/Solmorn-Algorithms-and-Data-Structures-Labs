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

    for c in [1, 2, 3, 4]:
        x, y = read_csv(f"results/results_introsort_c{c}.csv")
        plt.plot(x, y, label=f"C = {c}")

    plt.xlabel("Array size")
    plt.ylabel("Average time, sec")
    plt.title("Introsort: choice of C in C log n")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/plot_point7_c.svg", format="svg")
    plt.close()


if __name__ == "__main__":
    main()

    