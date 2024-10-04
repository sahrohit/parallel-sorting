import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Read the CSV file
df = pd.read_csv('benchmark_results.csv')

# Create different plots for analysis

# 1. Runtime vs Input Size for each algorithm
plt.figure(figsize=(12, 8))
for algo in df['Algorithm'].unique():
    data = df[df['Algorithm'] == algo]
    plt.plot(data['Input Size'], data['Runtime'], marker='o', label=algo)

plt.xscale('log')
plt.yscale('log')
plt.xlabel('Input Size')
plt.ylabel('Runtime (seconds)')
plt.title('Runtime vs Input Size')
plt.legend()
plt.savefig('runtime_comparison.png')
plt.close()

# 2. Speedup analysis for parallel algorithms
parallel_algos = [algo for algo in df['Algorithm'].unique() if 'p' in algo]
serial_algos = [algo.replace('p', 's') for algo in parallel_algos]

for p_algo, s_algo in zip(parallel_algos, serial_algos):
    plt.figure(figsize=(10, 6))
    
    for thread in df['Threads'].unique():
        p_data = df[(df['Algorithm'] == p_algo) & (df['Threads'] == thread)]
        s_data = df[df['Algorithm'] == s_algo]
        
        if not p_data.empty and not s_data.empty:
            speedup = [s/p for s, p in zip(s_data['Runtime'], p_data['Runtime'])]
            plt.plot(p_data['Input Size'], speedup, marker='o', label=f'{thread} threads')
    
    plt.xscale('log')
    plt.xlabel('Input Size')
    plt.ylabel('Speedup')
    plt.title(f'Speedup Analysis - {p_algo}')
    plt.legend()
    plt.savefig(f'speedup_{p_algo}.png')
    plt.close()

print("Analysis complete. Check the generated PNG files for visualizations.")