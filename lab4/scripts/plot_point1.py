import csv
import os
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

ROOT = Path(".")
RESULTS = ROOT / "results"
PLOTS = ROOT / "plots"
PLOTS.mkdir(parents=True, exist_ok=True)


def read_hist_csv(path: Path):
    with path.open("r", encoding="utf-8") as file:
        reader = csv.DictReader(file)
        rows = list(reader)
    columns = [name for name in reader.fieldnames if name != "bucket"]
    buckets = [int(row["bucket"]) for row in rows]
    data = {name: [int(row[name]) for row in rows] for name in columns}
    return buckets, data


def read_summary_csv(path: Path):
    with path.open("r", encoding="utf-8") as file:
        reader = csv.DictReader(file)
        rows = list(reader)
    return rows


def make_hist_plots(prefix: str, hist_name: str, title_prefix: str):
    buckets, data = read_hist_csv(RESULTS / hist_name)
    for name, values in data.items():
        plt.figure(figsize=(18, 6))
        plt.bar(buckets, values, width=1.0)
        plt.xlabel("bucket")
        plt.ylabel("collisions")
        plt.title(f"{title_prefix}: {name}")
        plt.xticks(range(0, 1001, 100))
        plt.tight_layout()
        plt.savefig(PLOTS / f"{prefix}_{name}.svg")
        plt.close()


def make_summary_plot(prefix: str, summary_name: str, value_key: str, ylabel: str, title: str):
    rows = read_summary_csv(RESULTS / summary_name)
    names = [row["hash_name"] for row in rows]
    values = [float(row[value_key]) for row in rows]

    plt.figure(figsize=(10, 6))
    plt.bar(names, values)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.xticks(rotation=20, ha="right")
    plt.tight_layout()
    plt.savefig(PLOTS / f"{prefix}_{value_key}.svg")
    plt.close()


def main():
    make_hist_plots("point1_uint", "results_point1_uint_hist.csv", "Point 1 uint histogram")
    make_hist_plots("point1_float", "results_point1_float_hist.csv", "Point 1 float histogram")
    make_hist_plots("point1_string", "results_point1_string_hist.csv", "Point 1 string histogram")

    make_summary_plot("point1_uint", "results_point1_uint_summary.csv", "variance", "variance", "Point 1 uint variance")
    make_summary_plot("point1_uint", "results_point1_uint_summary.csv", "time_seconds", "time, s", "Point 1 uint time")
    make_summary_plot("point1_float", "results_point1_float_summary.csv", "variance", "variance", "Point 1 float variance")
    make_summary_plot("point1_float", "results_point1_float_summary.csv", "time_seconds", "time, s", "Point 1 float time")
    make_summary_plot("point1_string", "results_point1_string_summary.csv", "variance", "variance", "Point 1 string variance")
    make_summary_plot("point1_string", "results_point1_string_summary.csv", "time_seconds", "time, s", "Point 1 string time")

    print("point 1 plots generated")


if __name__ == "__main__":
    main()
