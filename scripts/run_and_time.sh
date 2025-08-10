#!/bin/bash

# This script times the execution of the external sorting algorithms on 3 different input files.

# Set TIMEFORMAT for the 'time' keyword to output only the real seconds
export TIMEFORMAT=%R

# Clean up previous artifacts to ensure a fair timing run
echo "--- Cleaning up old files... ---"
rm -f run*.bin merge_pass*.bin partition_*.bin sorted_*.bin data/sorted_*.txt report/times.dat

# Ensure executables are built before timing
echo "--- Building executables... ---"
make merge_sort > /dev/null
make quick_sort > /dev/null

# Create the header for the data file
echo "Run,AlgorithmConfig,MergeSortTime,QuickSortTime" > report/times.dat

MS_EXEC="bin/merge_sort_exec"
QS_EXEC="bin/quick_sort_exec"
MEM_LIMIT="16777216"

# --- Configurations ---
MS_K_VALUES=("heuristic" 4 8 16)

declare -A QS_CONFIGS
QS_CONFIGS=(
    ["A"]="2 2 2 10"
    ["B"]="1 1 1 13"
    ["C"]="2 1 1 12"
)

# Loop through the 3 input files
for i in {1..3}
do
    INPUT_FILE="data/input_${i}.txt"
    echo "--- Processing file ${i}: ${INPUT_FILE} ---"

    # --- Run Merge Sort Experiments ---
    for k in "${MS_K_VALUES[@]}"; do
        MS_OUT_FILE="data/sorted_ms_${i}_k_${k}.txt"
        echo "  Timing External Merge Sort with K=${k}..."
        
        k_arg=""
        if [ "$k" != "heuristic" ]; then
            k_arg=$k
        fi

        ms_time=$( (time ${MS_EXEC} ${INPUT_FILE} ${MS_OUT_FILE} ${MEM_LIMIT} ${k_arg} >/dev/null) 2>&1 )
        if [ $? -ne 0 ]; then
            echo "  Merge Sort command failed for K=$k!"
            ms_time="-1.0"
        fi
        echo "  Merge Sort Time (K=$k): $ms_time seconds"

        # Append results to the data file
        echo "${i},K=${k},${ms_time}," >> report/times.dat
    done

    # --- Run Quick Sort Experiments ---
    for config_name in A B C; do # Iterate in a specific order
        params=${QS_CONFIGS[$config_name]}
        QS_OUT_FILE="data/sorted_qs_${i}_${config_name}.txt"
        echo "  Timing External Quick Sort with Config ${config_name}..."
        
        qs_time=$( (time ${QS_EXEC} ${INPUT_FILE} ${QS_OUT_FILE} ${MEM_LIMIT} ${params} >/dev/null) 2>&1 )
        if [ $? -ne 0 ]; then
            echo "  Quick Sort command failed for Config ${config_name}!"
            qs_time="-1.0"
        fi
        echo "  Quick Sort Time (Config ${config_name}): $qs_time seconds"

        # Append results to the data file
        echo "${i},QS_${config_name},,${qs_time}" >> report/times.dat
    done
done

echo "--- Timing complete. Results are in report/times.dat ---"
