from pathlib import Path
import csv
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

ROOT_DIR = Path(__file__).resolve().parents[1]

xs = []
times = []

with open(ROOT_DIR / "results" / "results_point2.csv", "r", encoding="utf-8") as file:
    reader = csv.DictReader(file)
    for row in reader:
        xs.append(int(row["n"]))
        times.append(float(row["binomial_build"]))

plt.figure(figsize=(10, 6))
plt.plot(xs, times, label="binomial build by inserts")
plt.xlabel("n")
plt.ylabel("time, s")
plt.title("Point 2: binomial heap build")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(ROOT_DIR / "plots" / "plot_point2.svg")

plt.figure(figsize=(10, 6))
plt.plot(xs, [t / x for t, x in zip(times, xs)], label="time / n")
plt.xlabel("n")
plt.ylabel("time / n")
plt.title("Point 2: normalized time")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig(ROOT_DIR / "plots" / "plot_point2_normalized.svg")
