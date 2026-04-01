import csv
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

xs = []
linear = []
insert_build = []

with open("./results/results_point1.csv", "r", encoding="utf-8") as file:
    reader = csv.DictReader(file)
    for row in reader:
        xs.append(int(row["n"]))
        linear.append(float(row["linear_build"]))
        insert_build.append(float(row["insert_build"]))

plt.figure(figsize=(10, 6))
plt.plot(xs, linear, label="linear heapify")
plt.plot(xs, insert_build, label="build by inserts")
plt.xlabel("n")
plt.ylabel("time, s")
plt.title("Point 1: binary heap build")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("./plots/plot_point1.svg")