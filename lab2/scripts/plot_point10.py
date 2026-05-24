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
    point1_x, point1_y = read_csv("results/results_shell.csv")
    point2_x, point2_y = read_csv("results/results_heap_k4.csv")
    point3_x, point3_y = read_csv("results/results_merge_iterative.csv")
    point4_x, point4_y = read_csv("results/results_quick_fat_one_branch_cutoff40.csv")
    point5_x, point5_y = read_csv("results/results_pivot_median3.csv")
    point6_x, point6_y = read_csv("results/results_shell_cutoff_t32.csv")
    point7_x, point7_y = read_csv("results/results_introsort_c2.csv")
    point8_x, point8_y = read_csv("results/results_pdqsort_github.csv")
    point9_x, point9_y = read_csv("results/results_lsd_radix.csv")
    qsort_x, qsort_y = read_csv("results/results_qsort_big.csv")

    Path("plots").mkdir(exist_ok=True)

    plt.figure(figsize=(12, 7))

    plt.plot(point1_x, point1_y, label="P1: Shell sort")
    plt.plot(point2_x, point2_y, label="P2: 4-ary heap sort")
    plt.plot(point3_x, point3_y, label="P3: Iterative merge sort")
    plt.plot(point4_x, point4_y, label="P4: Fat quick sort + 1 branch + cutoff40")
    plt.plot(point5_x, point5_y, label="P5: Pivot median of 3")
    plt.plot(point6_x, point6_y, label="P6: Quick + Shell cutoff32")
    plt.plot(point7_x, point7_y, label="P7: Introsort C=2")
    plt.plot(point8_x, point8_y, label="P8: PDQSort")
    plt.plot(point9_x, point9_y, label="P9: LSD radix sort")
    plt.plot(qsort_x, qsort_y, label="libc qsort")

    plt.xlabel("Array size")
    plt.ylabel("Average time, sec")
    plt.title("Best algorithms by points vs qsort on big_tests")
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.savefig("plots/plot_point10.svg", format="svg")
    plt.close()



if __name__ == "__main__":
    main()