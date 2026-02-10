import matplotlib.pyplot as plt
import os

data = {
    "Recursive": {
        "n": [5, 10, 15, 20, 25, 30, 35, 40],
        "time": [0.00670467, 0.009219, 0.086975, 0.966478, 10.8752, 119.011, 1317.94, 14584.7]
    },
    "Dynamic": {
        "n": [10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000],
        "time": [1.5943, 8.54387, 20.1353, 38.3036, 62.415, 90.0258, 124.238, 158.372, 196.054, 254.374]
    },
    "Matrix": {
        "n": [10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000],
        "time": [3.84624, 13.6129, 27.5083, 45.6946, 69.0361, 95.5635, 126.621, 162.178, 202.031, 245.716]
    },
    "Binet": {
        "n": [20, 40, 60, 80, 100, 120, 140],
        "time": [0.012222, 0.00142, 0.00130333, 0.001257, 0.00142, 0.00160633, 0.001257]
    },
    "Doubling": {
        "n": [1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000, 10000000],
        "time": [3.19612, 7.44411, 11.8371, 16.3989, 21.5806, 28.1953, 34.498, 37.0686, 45.1215, 47.1681]
    }
}

output_dir = "graphRepresentations"
os.makedirs(output_dir, exist_ok=True)


for method, vals in data.items():
    plt.figure(figsize=(6, 6))
    plt.plot(vals["n"], vals["time"], marker='o')
    plt.title(f"{method} Method Results")
    plt.xlabel("n")
    plt.ylabel("Avg Time (ms)")
    plt.grid(True)
    if method == "Doubling":
        # Format Doubling x-axis labels with commas
        plt.xticks(vals["n"], [f"{x:,}" for x in vals["n"]], rotation=30)
    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, f"{method.lower()}_results.png"))
    plt.close()

print("Graphs saved in graphRepresentations/")
