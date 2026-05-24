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
    insertion_x, insertion_y = read_csv("results/results_insertion.csv")
    bubble_x, bubble_y = read_csv("results/results_bubble.csv")
    selection_x, selection_y = read_csv("results/results_selection.csv")
    shell_x, shell_y = read_csv("results/results_shell.csv")

    Path("plots").mkdir(exist_ok=True)

    plt.figure(figsize=(10, 6))

    plt.plot(insertion_x, insertion_y, label="Insertion sort")
    plt.plot(bubble_x, bubble_y, label="Bubble sort")
    plt.plot(selection_x, selection_y, label="Selection sort")
    plt.plot(shell_x, shell_y, label="Shell sort (Knuth sequence)")

    plt.xlabel("Array size")
    plt.ylabel("Average time, sec")
    plt.title("Comparison of quadratic sorts on small_tests")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/plot_point1.svg", format="svg")
    plt.close()


if __name__ == "__main__":
    main()