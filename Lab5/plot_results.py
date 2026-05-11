import csv
from pathlib import Path

import matplotlib.pyplot as plt


def read_csv(path):
    sizes = []
    edges = []
    prim_ms = []
    kruskal_ms = []

    with open(path, newline="") as handle:
        reader = csv.DictReader(handle)
        for row in reader:
            sizes.append(int(row["vertices"]))
            edges.append(int(row["edges"]))
            prim_ms.append(float(row["prim_ms"]))
            kruskal_ms.append(float(row["kruskal_ms"]))


    return sizes, edges, prim_ms, kruskal_ms


def make_plot(csv_path, out_path, title):

    sizes, edges, prim_ms, kruskal_ms = read_csv(csv_path)

    fig, ax = plt.subplots(figsize=(8, 5))
    ax.plot(sizes, prim_ms, marker="o", label="Prim")
    ax.plot(sizes, kruskal_ms, marker="s", label="Kruskal")

    ax.set_title(title)
    ax.set_xlabel("Vertices")
    ax.set_ylabel("Time (ms)")
    ax.grid(True, alpha=0.3)
    ax.legend()

    fig.tight_layout()
    fig.savefig(out_path, dpi=150)


def make_compare_plot(sizes, unopt_vals, opt_vals, out_path, title, y_label):
    fig, ax = plt.subplots(figsize=(8, 5))
    ax.plot(sizes, unopt_vals, marker="o", label="Unoptimised")
    ax.plot(sizes, opt_vals, marker="s", label="Optimised")

    ax.set_title(title)
    ax.set_xlabel("Vertices")
    ax.set_ylabel(y_label)
    ax.grid(True, alpha=0.3)
    ax.legend()

    fig.tight_layout()
    fig.savefig(out_path, dpi=150)


def main():
    here = Path(__file__).resolve().parent
    unopt = here / "unoptimised.csv"
    opt = here / "optimised.csv"

    if not unopt.exists() or not opt.exists():
        print("Missing CSV files. Run the C++ program first.")
        return

    make_plot(unopt, here / "unoptimised.png", "Unoptimised MST Times")
    make_plot(opt, here / "optimised.png", "Optimised MST Times")

    sizes_u, edges_u, prim_u, kruskal_u = read_csv(unopt)
    sizes_o, edges_o, prim_o, kruskal_o = read_csv(opt)

    make_compare_plot(
        sizes_u,
        prim_u,
        prim_o,
        here / "prim_compare.png",
        "Prim: Unoptimised vs Optimised",
        "Time (ms)",
    )

    make_compare_plot(
        sizes_u,
        kruskal_u,
        kruskal_o,
        here / "kruskal_compare.png",
        "Kruskal: Unoptimised vs Optimised",
        "Time (ms)",
    )

    print("Wrote unoptimised.png, optimised.png, prim_compare.png, kruskal_compare.png")


if __name__ == "__main__":
    main()
