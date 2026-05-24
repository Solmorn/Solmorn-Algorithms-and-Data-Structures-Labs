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

    plt.figure(figsize=(11, 6))

    for k in range(2, 11):
        x, y = read_csv(f"results/results_heap_k{k}.csv")
        plt.plot(x, y, label=f"{k}-ary heap")

    plt.xlabel("Array size")
    plt.ylabel("Average time, sec")
    plt.title("Bottom-up k-ary heapsort comparison on big_tests")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/plot_point2.svg", format="svg")
    plt.close()


if __name__ == "__main__":
    main()