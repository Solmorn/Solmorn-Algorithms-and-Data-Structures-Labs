#!/usr/bin/env python3
import csv
from pathlib import Path

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

ROOT = Path('.')
RESULTS = ROOT / 'results'
PLOTS = ROOT / 'plots'
PLOTS.mkdir(parents=True, exist_ok=True)


def read_summary():
    rows = []
    with (RESULTS / 'results_point3_summary.csv').open('r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            rows.append(row)
    return rows


def plot_build_times(rows):
    tables = [row['table'] for row in rows]
    values = [float(row['build_time_seconds']) for row in rows]

    plt.figure(figsize=(10, 6))
    plt.bar(tables, values)
    plt.title('Point 3: build time')
    plt.xlabel('hash table')
    plt.ylabel('time, s')
    plt.grid(True, axis='y')
    plt.tight_layout()
    plt.savefig(PLOTS / 'point3_build_time.svg')
    plt.close()


def plot_search_times(rows):
    tables = [row['table'] for row in rows]
    values = [float(row['search_time_seconds']) for row in rows]

    plt.figure(figsize=(10, 6))
    plt.bar(tables, values)
    plt.title('Point 3: search time for 10,000,000 queries')
    plt.xlabel('hash table')
    plt.ylabel('time, s')
    plt.grid(True, axis='y')
    plt.tight_layout()
    plt.savefig(PLOTS / 'point3_search_time.svg')
    plt.close()


def main():
    rows = read_summary()
    plot_build_times(rows)
    plot_search_times(rows)
    print('point 3 plots saved to ./plots')


if __name__ == '__main__':
    main()