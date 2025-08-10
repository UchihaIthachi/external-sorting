import matplotlib.pyplot as plt
import pandas as pd
import os

DATA_FILE = 'report/times.dat'

def plot_merge_sort_time_by_k(df):
    # Filter for merge sort data
    ms_df = df[df['AlgorithmConfig'].str.startswith('K=')].copy()
    if ms_df.empty:
        print("No Merge Sort data to plot.")
        return

    ms_df['K'] = ms_df['AlgorithmConfig'].str.replace('K=', '')
    ms_df['MergeSortTime'] = pd.to_numeric(ms_df['MergeSortTime'], errors='coerce').mask(lambda x: x < 0)
    
    k_order = ['heuristic', '4', '8', '16']
    ms_df['K'] = pd.Categorical(ms_df['K'], categories=k_order, ordered=True)

    pivot_df = ms_df.pivot(index='Run', columns='K', values='MergeSortTime')
    if pivot_df.empty:
        print("Not enough data to plot Merge Sort time by K.")
        return

    ax = pivot_df.plot(kind='bar', figsize=(12, 7), colormap='viridis', width=0.8)
    for container in ax.containers:
        ax.bar_label(container, fmt='%.2f', label_type='edge', fontsize=9, padding=3)

    plt.title('Merge Sort Performance by K value')
    plt.xlabel('Test Run')
    plt.ylabel('Execution Time (s)')
    ax.set_xticklabels([f'Run {i}' for i in pivot_df.index], rotation=0)
    ymin, ymax = ax.get_ylim()
    ax.set_ylim(0, ymax * 1.15)
    plt.legend(title='K value')
    plt.tight_layout()
    output_path = 'report/figures/merge_sort_time.png'
    plt.savefig(output_path)
    print(f"Plot saved to {output_path}")
    plt.close()

def plot_quick_sort_time_by_config(df):
    qs_df = df[df['AlgorithmConfig'].str.startswith('QS_')].copy()
    if qs_df.empty:
        print("No Quick Sort data to plot.")
        return

    qs_df['Config'] = qs_df['AlgorithmConfig'].str.replace('QS_', '')
    qs_df['QuickSortTime'] = pd.to_numeric(qs_df['QuickSortTime'], errors='coerce').mask(lambda x: x < 0)

    config_order = ['A', 'B', 'C']
    qs_df['Config'] = pd.Categorical(qs_df['Config'], categories=config_order, ordered=True)

    pivot_df = qs_df.pivot(index='Run', columns='Config', values='QuickSortTime')
    if pivot_df.empty:
        print("Not enough data to plot Quick Sort time by config.")
        return

    ax = pivot_df.plot(kind='bar', figsize=(12, 7), colormap='plasma', width=0.8)
    for container in ax.containers:
        ax.bar_label(container, fmt='%.2f', label_type='edge', fontsize=9, padding=3)

    plt.title('Quick Sort Performance by Configuration')
    plt.xlabel('Test Run')
    plt.ylabel('Execution Time (s)')
    ax.set_xticklabels([f'Run {i}' for i in pivot_df.index], rotation=0)
    ymin, ymax = ax.get_ylim()
    ax.set_ylim(0, ymax * 1.15)
    plt.legend(title='Configuration')
    plt.tight_layout()
    output_path = 'report/figures/quick_sort_time.png'
    plt.savefig(output_path)
    print(f"Plot saved to {output_path}")
    plt.close()

def plot_best_comparison(df):
    ms_df = df[df['AlgorithmConfig'].str.startswith('K=')].copy()
    qs_df = df[df['AlgorithmConfig'].str.startswith('QS_')].copy()

    if ms_df.empty or qs_df.empty:
        print("Not enough data for best vs best comparison plot.")
        return

    ms_df['MergeSortTime'] = pd.to_numeric(ms_df['MergeSortTime'], errors='coerce').dropna()
    qs_df['QuickSortTime'] = pd.to_numeric(qs_df['QuickSortTime'], errors='coerce').dropna()

    if ms_df.empty or qs_df.empty:
        print("Not enough valid time data for best vs best comparison plot.")
        return

    best_ms = ms_df.loc[ms_df.groupby('Run')['MergeSortTime'].idxmin()]
    best_qs = qs_df.loc[qs_df.groupby('Run')['QuickSortTime'].idxmin()]

    comparison_df = pd.merge(best_ms[['Run', 'MergeSortTime']], best_qs[['Run', 'QuickSortTime']], on='Run', how='outer')
    comparison_df.set_index('Run', inplace=True)

    ax = comparison_df.plot(kind='bar', figsize=(10, 6), width=0.8)
    for container in ax.containers:
        ax.bar_label(container, fmt='%.2f', label_type='edge', fontsize=9, padding=3)
    
    plt.title('Best Performance: Merge Sort vs. Quick Sort')
    plt.ylabel('Execution Time (s)')
    plt.xlabel('Test Run')
    ax.set_xticklabels([f'Run {i}' for i in comparison_df.index], rotation=0)
    ymin, ymax = ax.get_ylim()
    ax.set_ylim(0, ymax * 1.15)
    plt.legend(['Best Merge Sort', 'Best Quick Sort'])
    plt.tight_layout()
    output_path = 'report/figures/best_vs_best.png'
    plt.savefig(output_path)
    print(f"Plot saved to {output_path}")
    plt.close()


def main():
    if not os.path.exists(DATA_FILE):
        print(f"Data file not found: {DATA_FILE}")
        return

    try:
        df = pd.read_csv(DATA_FILE)
        if df.empty:
            print(f"Data file is empty: {DATA_FILE}")
            return
    except pd.errors.EmptyDataError:
        print(f"Data file is empty: {DATA_FILE}")
        return

    plot_merge_sort_time_by_k(df)
    plot_quick_sort_time_by_config(df)
    plot_best_comparison(df)

if __name__ == '__main__':
    main()
