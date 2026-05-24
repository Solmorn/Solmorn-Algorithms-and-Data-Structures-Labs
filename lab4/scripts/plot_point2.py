#!/usr/bin/env python3
import csv
from collections import defaultdict
from pathlib import Path

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

ROOT = Path('.')
RESULTS = ROOT / 'results'
PLOTS = ROOT / 'plots'
PLOTS.mkdir(parents=True, exist_ok=True)


def read_grouped_csv(path, x_field):
    grouped = defaultdict(lambda: {'x': [], 'y': []})
    with path.open('r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            table = row['table']
            grouped[table]['x'].append(float(row[x_field]))
            grouped[table]['y'].append(float(row['time_seconds']))
    return grouped


def plot_load_factor():
    grouped = read_grouped_csv(RESULTS / 'results_point2_load_factor.csv', 'load_factor')
    plt.figure(figsize=(10, 6))
    for table, values in grouped.items():
        plt.plot(values['x'], values['y'], marker='o', label=table)
    plt.title('Point 2: insert time vs load factor')
    plt.xlabel('load factor')
    plt.ylabel('time, s')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(PLOTS / 'point2_load_factor.svg')
    plt.close()


def plot_ops(filename, output_name, title):
    grouped = read_grouped_csv(RESULTS / filename, 'operations')
    plt.figure(figsize=(10, 6))
    for table, values in grouped.items():
        plt.plot(values['x'], values['y'], label=table)
    plt.title(title)
    plt.xlabel('number of operations')
    plt.ylabel('time, s')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(PLOTS / output_name)
    plt.close()


def main():
    plot_load_factor()
    plot_ops(
        'results_point2_ops_equal.csv',
        'point2_ops_equal.svg',
        'Point 2: random operations, p(insert)=p(find)=p(erase)=1/3',
    )
    plot_ops(
        'results_point2_ops_bias.csv',
        'point2_ops_bias.svg',
        'Point 2: random operations, p(insert)=0.5, p(find)=p(erase)=0.25',
    )
    print('point 2 plots saved to ./plots')


if __name__ == '__main__':
    main()



