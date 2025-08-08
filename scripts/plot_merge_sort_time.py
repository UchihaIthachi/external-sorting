import matplotlib.pyplot as plt
import pandas as pd
import os

DATA_FILE = 'report/times.dat'

def plot_times():
    if not os.path.exists(DATA_FILE):
        print(f"Data file not found: {DATA_FILE}")
        return

    # Read the CSV data
    try:
        df = pd.read_csv(DATA_FILE)
    except pd.errors.EmptyDataError:
        print(f"Data file is empty: {DATA_FILE}")
        return


    # Filter out failed runs for plotting
    successful_runs = df[df['MergeSortTime'] >= 0]

    if successful_runs.empty:
        print("No successful merge sort runs to plot.")
        # Create an empty plot as a placeholder
        plt.figure(figsize=(8, 5))
        plt.text(0.5, 0.5, 'No successful runs to plot', ha='center', va='center')
        plt.title('Merge Sort Execution Time')
        plt.xlabel('Test Run')
        plt.ylabel('Execution Time (s)')
    else:
        run_labels = [f"Run {i}" for i in successful_runs['Run']]
        merge_sort_times = successful_runs['MergeSortTime']

        plt.figure(figsize=(8, 5))
        bars = plt.bar(run_labels, merge_sort_times, color='skyblue', label='Merge Sort')
        
        # Add time labels on top of bars
        for bar in bars:
            yval = bar.get_height()
            plt.text(bar.get_x() + bar.get_width()/2.0, yval, f'{yval:.2f}s', va='bottom', ha='center')

        plt.ylabel('Execution Time (s)')
        plt.xlabel('Test Run')
        plt.title('Execution Time of Merge Sort Across 3 Runs (256MB Input)')
        if not merge_sort_times.empty:
            plt.ylim(0, max(merge_sort_times) * 1.2) # Add some space at the top
        plt.legend()
    
    output_path = 'report/figures/merge_sort_time.png'
    plt.savefig(output_path)
    print(f"Plot saved to {output_path}")
    plt.close()

if __name__ == '__main__':
    plot_times()
