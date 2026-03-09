import os
import pandas as pd
import matplotlib.pyplot as plt

# Settings
base_dir = '/home/gabi/University Stuff/AA sem 4/Lab 3'
traverse_dir = os.path.join(base_dir, 'traverse')
find_dir = os.path.join(base_dir, 'find')
traverse_graphs_dir = os.path.join(base_dir, 'traverseGraphs')
find_graphs_dir = os.path.join(base_dir, 'findGraphs')

os.makedirs(traverse_graphs_dir, exist_ok=True)
os.makedirs(find_graphs_dir, exist_ok=True)

def plot_traverse(graph_type, algo):
    csv_path = os.path.join(traverse_dir, graph_type, f'{algo}.csv')
    if not os.path.exists(csv_path):
        return
    # Read all lines
    with open(csv_path) as f:
        lines = f.readlines()
    # Find the line with maxMemory
    maxmem_line = [l for l in lines if l.startswith('maxMemory')]
    if maxmem_line:
        maxmems = [int(x) for x in maxmem_line[0].strip().split(',')[1:]]
    else:
        maxmems = None
    # Read only the data lines (skip the last line if it's maxMemory)
    data_lines = [l for l in lines if not l.startswith('maxMemory')]
    from io import StringIO
    df = pd.read_csv(StringIO(''.join(data_lines)))
    plt.figure(figsize=(8,5))
    ms_times = df['Time_us'] / 1000.0
    plt.plot(df['Size'], ms_times, marker='o', label='Traversal Time (ms)')
    plt.xlabel('Graph Size')
    plt.ylabel('Time (ms)')
    plt.title(f'{algo} on {graph_type} (Traversal)')
    plt.grid(True)
    if maxmems:
        for i, (x, m) in enumerate(zip(df['Size'], maxmems)):
            plt.annotate(f'MaxMem={m}', (x, ms_times[i]), textcoords="offset points", xytext=(0,10), ha='center', fontsize=8, color='green')
        plt.figtext(0.99, 0.01, f'Max memory: {maxmems}', ha='right', fontsize=8, color='green')
    plt.legend()
    outdir = os.path.join(traverse_graphs_dir, graph_type)
    os.makedirs(outdir, exist_ok=True)
    plt.savefig(os.path.join(outdir, f'{algo}.png'))
    plt.close()

def plot_find(graph_type, algo):
    csv_path = os.path.join(find_dir, graph_type, f'{algo}.csv')
    if not os.path.exists(csv_path):
        return
    df = pd.read_csv(csv_path)
    plt.figure(figsize=(8,5))
    ms_times = df['AvgFindTime_us'] / 1000.0
    plt.plot(df['Size'], ms_times, marker='o', label='Avg Find Time (ms)')
    plt.xlabel('Graph Size')
    plt.ylabel('Avg Find Time (ms)')
    plt.title(f'{algo} on {graph_type} (Find)')
    plt.grid(True)
    plt.legend()
    outdir = os.path.join(find_graphs_dir, graph_type)
    os.makedirs(outdir, exist_ok=True)
    plt.savefig(os.path.join(outdir, f'{algo}.png'))
    plt.close()

graph_types = [d for d in os.listdir(traverse_dir) if os.path.isdir(os.path.join(traverse_dir, d))]
algos = ['DFS', 'DFSo', 'BFS', 'BFSo']

for gtype in graph_types:
    for algo in algos:
        plot_traverse(gtype, algo)
        plot_find(gtype, algo)

print('All graphs generated in traverseGraphs/ and findGraphs/.')
