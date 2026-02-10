import matplotlib.pyplot as plt
import os

# Ensure output directory exists
output_dir = "diagrams"
os.makedirs(output_dir, exist_ok=True)

# Data structure
algorithms = [
    "BubbleSort", "SelectionSort", "InsertionSort", "MergeSort", "QuickSort"
]
data_types = ["Random", "Sorted", "Reverse Sorted", "High Duplicates"]

data = {
    "BubbleSort": {
        "Size": [10, 100, 1000, 10000],
        "Random": [0.001, 0.022, 1.516, 158.242],
        "Sorted": [0.001, 0.001, 0.002, 0.006],
        "Reverse Sorted": [0.001, 0.029, 2.585, 258.518],
        "High Duplicates": [0.002, 0.021, 1.544, 157.748]
    },
    "SelectionSort": {
        "Size": [10, 100, 1000, 10000],
        "Random": [0.002, 0.007, 0.308, 25.467],
        "Sorted": [0.001, 0.005, 0.253, 24.539],
        "Reverse Sorted": [0.001, 0.005, 0.289, 25.184],
        "High Duplicates": [0.002, 0.007, 0.297, 25.018]
    },
    "InsertionSort": {
        "Size": [10, 100, 1000, 10000],
        "Random": [0.001, 0.003, 0.110, 10.248],
        "Sorted": [0.001, 0.001, 0.002, 0.014],
        "Reverse Sorted": [0.003, 0.004, 0.231, 20.462],
        "High Duplicates": [0.001, 0.003, 0.112, 10.261]
    },
    "MergeSort": {
        "Size": [10, 100, 1000, 10000, 100000, 1000000, 10000000],
        "Random": [0.002, 0.004, 0.042, 0.537, 6.530, 82.384, 984.554],
        "Sorted": [0.001, 0.002, 0.009, 0.091, 1.025, 15.294, 191.422],
        "Reverse Sorted": [0.001, 0.002, 0.010, 0.109, 1.194, 17.460, 224.765],
        "High Duplicates": [0.001, 0.004, 0.038, 0.405, 4.233, 48.485, 574.867]
    },
    "QuickSort": {
        "Size": [10, 100, 1000, 10000, 100000, 1000000, 10000000],
        "Random": [0.001, 0.004, 0.039, 0.477, 5.777, 67.503, 770.164],
        "Sorted": [0.001, 0.002, 0.008, 0.079, 0.900, 10.370, 121.321],
        "Reverse Sorted": [0.001, 0.002, 0.010, 0.107, 1.225, 14.151, 177.671],
        "High Duplicates": [0.001, 0.004, 0.036, 0.327, 3.283, 32.709, 332.522]
    }
}


# One graph per algorithm, all data types
for algo in algorithms:
    plt.figure(figsize=(8, 6))
    for dtype in data_types:
        plt.plot(data[algo]["Size"], data[algo][dtype], marker='o', label=dtype)
    plt.title(f"{algo} - All Data Types")
    plt.xlabel("Array Size")
    plt.ylabel("Time (ms)")
    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"{output_dir}/{algo}_AllDataTypes.png")
    plt.close()

# Summary graphs for each data type (all algorithms)
for dtype in data_types:
    plt.figure(figsize=(8, 6))
    for algo in algorithms:
        # Only plot sizes that exist for the algorithm
        plt.plot(data[algo]["Size"], data[algo][dtype], marker='o', label=algo)
    plt.title(f"All Algorithms - {dtype}")
    plt.xlabel("Array Size")
    plt.ylabel("Time (ms)")
    plt.xscale("log")
    plt.yscale("log")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"{output_dir}/AllAlgorithms_{dtype.replace(' ', '_')}.png")
    plt.close()
