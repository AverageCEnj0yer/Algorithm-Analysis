import matplotlib.pyplot as plt
import os
import pandas as pd

# Plot: Comparison for each algorithm in random_timing
random_timing_dir = "random_timing"
random_algos = [
    ("BubbleSort", "SteroidsBubbleSort"),
    ("MergeSort", "SteroidsMergeSort"),
    ("QuickSort", "SteroidsQuickSort"),
    ("HeapSort", "SteroidsHeapSort")
]

normal_dir = "diagrams"
steroids_dir = "diagrams_steroids"

for base, steroids in random_algos:
    # Only use current naming: normal and steroids
    # Plot four lines: O3 BubbleSort, O3 SteroidsBubbleSort, SteroidsBubbleSort (diagrams_steroids), BubbleSort (diagrams)
    sources = [
        (os.path.join(random_timing_dir, f"{base}.csv"), f"O3 {base}"),
        (os.path.join(random_timing_dir, f"{steroids}.csv"), f"O3 {steroids}"),
        (os.path.join(steroids_dir, f"{steroids}.csv"), f"Steroids {base}"),
        (os.path.join(normal_dir, f"{base}.csv"), f"Normal {base}")
    ]
    plt.figure(figsize=(8, 6))
    for csv_path, label in sources:
        if os.path.exists(csv_path):
            df = pd.read_csv(csv_path)
            if "Random" in df.columns:
                plt.plot(df["Size"], df["Random"], marker='o', label=label)
    plt.title(f"{base} - Random Timing Comparison")
    plt.xlabel("Array Size")
    plt.ylabel("Time (ms)")
    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"{random_timing_dir}/{base}_RandomTimingComparison.png")
    plt.close()

normal_dir = "diagrams"
steroids_dir = "diagrams_steroids"

algorithms = [
    "BubbleSort", "MergeSort", "QuickSort", "HeapSort",
    "SteroidsBubbleSort", "SteroidsMergeSort", "SteroidsQuickSort", "SteroidsHeapSort"
]
data_types = ["Random", "Sorted", "Reverse Sorted", "High Duplicates"]

# Read CSVs into a dictionary of DataFrames
results = {}
for algo in algorithms:
    for folder in [normal_dir, steroids_dir]:
        csv_path = os.path.join(folder, f"{algo}.csv")
        if os.path.exists(csv_path):
            df = pd.read_csv(csv_path)
            results[algo] = df


# Plot: One graph per algorithm, all data types
non_steroids = ["BubbleSort", "MergeSort", "QuickSort", "HeapSort"]
steroids = ["SteroidsBubbleSort", "SteroidsMergeSort", "SteroidsQuickSort", "SteroidsHeapSort"]

for algo in non_steroids:
    if algo not in results:
        continue
    df = results[algo]
    plt.figure(figsize=(8, 6))
    for dtype in data_types:
        if dtype in df.columns:
            plt.plot(df["Size"], df[dtype], marker='o', label=dtype)
    plt.title(f"{algo} - All Data Types")
    plt.xlabel("Array Size")
    plt.ylabel("Time (ms)")
    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"{normal_dir}/{algo}_AllDataTypes.png")
    plt.close()

for algo in steroids:
    if algo not in results:
        continue
    df = results[algo]
    plt.figure(figsize=(8, 6))
    for dtype in data_types:
        if dtype in df.columns:
            plt.plot(df["Size"], df[dtype], marker='o', label=dtype)
    plt.title(f"{algo} - All Data Types")
    plt.xlabel("Array Size")
    plt.ylabel("Time (ms)")
    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"{steroids_dir}/{algo}_AllDataTypes.png")
    plt.close()


# Plot: One graph per data type, all algorithms (non-steroids)
non_steroids = ["BubbleSort", "MergeSort", "QuickSort", "HeapSort"]
steroids = ["SteroidsBubbleSort", "SteroidsMergeSort", "SteroidsQuickSort", "SteroidsHeapSort"]

for dtype in data_types:
    # Non-steroids
    plt.figure(figsize=(8, 6))
    for algo in non_steroids:
        if algo not in results:
            continue
        df = results[algo]
        if dtype in df.columns:
            plt.plot(df["Size"], df[dtype], marker='o', label=algo)
    plt.title(f"All Algorithms (Non-Steroids) - {dtype}")
    plt.xlabel("Array Size")
    plt.ylabel("Time (ms)")
    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"{normal_dir}/AllAlgorithms_NonSteroids_{dtype.replace(' ', '_')}.png")
    plt.close()

    # Steroids
    plt.figure(figsize=(8, 6))
    for algo in steroids:
        if algo not in results:
            continue
        df = results[algo]
        if dtype in df.columns:
            plt.plot(df["Size"], df[dtype], marker='o', label=algo)
    plt.title(f"All Algorithms (Steroids) - {dtype}")
    plt.xlabel("Array Size")
    plt.ylabel("Time (ms)")
    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"{steroids_dir}/AllAlgorithms_Steroids_{dtype.replace(' ', '_')}.png")
    plt.close()
