import matplotlib.pyplot as plt
import pandas as pd
import os

DATA_FILE = 'report/times.dat'

def plot_times_comparison():
    if not os.path.exists(DATA_FILE):
        print(f"Data file not found: {DATA_FILE}")
        return

    # Read the CSV data
    try:
        df = pd.read_csv(DATA_FILE)
    except pd.errors.EmptyDataError:
        print(f"Data file is empty: {DATA_FILE}")
        return

    # Prepare run labels and times, converting -1 and failed runs to NaN for plotting
    runs = [f"Run {int(i)}" for i in df['Run']]
    ms_times = pd.to_numeric(df['MergeSortTime'], errors='coerce')
    qs_times = pd.to_numeric(df['QuickSortTime'], errors='coerce')
    ms_times = ms_times.mask(ms_times < 0)
    qs_times = qs_times.mask(qs_times < 0)

    x = range(len(runs))
    width = 0.35  # Bar width

    plt.figure(figsize=(9, 5))
    bar1 = plt.bar([i - width/2 for i in x], ms_times, width, label='Merge Sort', color='skyblue')
    bar2 = plt.bar([i + width/2 for i in x], qs_times, width, label='Quick Sort', color='orange')

    # Add time labels on top of bars
    for bars in [bar1, bar2]:
        for bar in bars:
            yval = bar.get_height()
            if not pd.isna(yval):
                plt.text(bar.get_x() + bar.get_width()/2.0, yval, f'{yval:.2f}s', va='bottom', ha='center', fontsize=9)

    plt.xticks(x, runs)
    plt.ylabel('Execution Time (s)')
    plt.xlabel('Test Run')
    plt.title('Execution Time Comparison: Merge Sort vs Quick Sort (256MB Input)')
    plt.legend()
    ymax = max([y for y in list(ms_times.dropna()) + list(qs_times.dropna())] + [1])
    plt.ylim(0, ymax * 1.2)

    output_path = 'report/figures/sort_times_comparison.png'
    plt.tight_layout()
    plt.savefig(output_path)
    print(f"Plot saved to {output_path}")
    plt.close()

if __name__ == '__main__':
    plot_times_comparison()
