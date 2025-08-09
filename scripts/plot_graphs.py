import matplotlib.pyplot as plt
import pandas as pd
import os

DATA_FILE = 'report/times.dat'

def plot_times_comparison(df):
    # Prepare run labels and times, converting -1 and failed runs to NaN for plotting
    runs = [f"Run {int(i)}" for i in df['Run']]
    ms_times = pd.to_numeric(df['MergeSortTime'], errors='coerce').mask(lambda x: x < 0)
    qs_times = pd.to_numeric(df['QuickSortTime'], errors='coerce').mask(lambda x: x < 0)

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
    
    # Safely calculate ymax
    valid_times = list(ms_times.dropna()) + list(qs_times.dropna())
    ymax = max(valid_times) if valid_times else 1
    plt.ylim(0, ymax * 1.2)

    output_path = 'report/figures/sort_times_comparison.png'
    plt.tight_layout()
    plt.savefig(output_path)
    print(f"Plot saved to {output_path}")
    plt.close()

def plot_performance_variability(df):
    ms_times = pd.to_numeric(df['MergeSortTime'], errors='coerce').mask(lambda x: x < 0).dropna()
    qs_times = pd.to_numeric(df['QuickSortTime'], errors='coerce').mask(lambda x: x < 0).dropna()
    
    data_to_plot = []
    labels = []
    if not ms_times.empty:
        data_to_plot.append(ms_times)
        labels.append('Merge Sort')
    if not qs_times.empty:
        data_to_plot.append(qs_times)
        labels.append('Quick Sort')

    if not data_to_plot:
        print("No valid data to plot for performance variability.")
        return

    plt.figure(figsize=(7, 5))
    plt.boxplot(data_to_plot, labels=labels)
    plt.ylabel('Execution Time (s)')
    plt.title('Performance Variability (256MB Input)')
    
    output_path = 'report/figures/performance_variability.png'
    plt.tight_layout()
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

    plot_times_comparison(df)
    plot_performance_variability(df)

if __name__ == '__main__':
    main()
