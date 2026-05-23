from pathlib import Path
import csv
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

ROOT_DIR = Path(__file__).resolve().parents[1]

def read_csv(path):
    xs = []
    naive = []
    binary = []
    binomial = []
    fibonacci = []

    with open(path, "r", encoding="utf-8") as file:
        reader = csv.DictReader(file)
        for row in reader:
            xs.append(int(row["n"]))
            naive.append(float(row["naive"]))
            binary.append(float(row["binary"]))
            binomial.append(float(row["binomial"]))
            fibonacci.append(float(row["fibonacci"]))

    return xs, naive, binary, binomial, fibonacci

def plot_file(path, title, output):
    xs, naive, binary, binomial, fibonacci = read_csv(path)

    plt.figure(figsize=(10, 6))
    plt.plot(xs, naive, label="naive")
    plt.plot(xs, binary, label="binary heap")
    plt.plot(xs, binomial, label="binomial heap")
    plt.plot(xs, fibonacci, label="fibonacci heap")
    plt.xlabel("n")
    plt.ylabel("time, s")
    plt.title(title)
    plt.yscale("log")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(output)

plot_file(
    ROOT_DIR / "results" / "results_point3_sparse.csv",
    "Point 3: Dijkstra on sparse graphs",
    ROOT_DIR / "plots" / "plot_point3_sparse.svg"
)

plot_file(
    ROOT_DIR / "results" / "results_point3_dense.csv",
    "Point 3: Dijkstra on dense graphs",
    ROOT_DIR / "plots" / "plot_point3_dense.svg"
)
