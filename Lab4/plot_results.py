import csv
from pathlib import Path
import matplotlib.pyplot as plt


def read_csv(path: Path):
    nodes = []
    times = []
    with path.open(newline="") as file:
        reader = csv.DictReader(file)
        for row in reader:
            nodes.append(int(row["nodes"]))
            times.append(float(row["avg_ms"]))
    return nodes, times


def plot_series(ax, title, series):
    for label, path in series:
        nodes, times = read_csv(path)
        ax.plot(nodes, times, marker="o", linewidth=2, label=label)
    ax.set_title(title)
    ax.set_xlabel("Nodes")
    ax.set_ylabel("Avg time (ms)")
    ax.grid(True, linestyle="--", alpha=0.4)
    ax.legend()



def main():
    base_dir = Path(__file__).resolve().parent
    csv_dir = base_dir / "csvFiles"

    dijkstra_series = [
        ("dense (p=0.8)", csv_dir / "dijkstra_dense.csv"),
        ("semi-dense (p=0.5)", csv_dir / "dijkstra_semi_dense.csv"),
        ("sparse (p=0.15)", csv_dir / "dijkstra_sparse.csv"),
    ]

    floyd_series = [
        ("dense (p=0.8)", csv_dir / "Floyd_dense.csv"),
        ("semi-dense (p=0.5)", csv_dir / "Floyd_semi_dense.csv"),
        ("sparse (p=0.15)", csv_dir / "Floyd_sparse.csv"),
    ]

    # Dijkstra plot
    fig1, ax1 = plt.subplots(figsize=(8, 6))
    plot_series(ax1, "Dijkstra", dijkstra_series)
    dijkstra_path = base_dir / "dijkstra_timings.png"
    fig1.savefig(dijkstra_path, dpi=150)

    # Floyd-Warshall plot
    fig2, ax2 = plt.subplots(figsize=(8, 6))
    plot_series(ax2, "Floyd-Warshall", floyd_series)
    floyd_path = base_dir / "floyd_timings.png"
    fig2.savefig(floyd_path, dpi=150)

    plt.show()


if __name__ == "__main__":
    main()
