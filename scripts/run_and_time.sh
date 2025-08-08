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
echo "Run,MergeSortTime,QuickSortTime" > report/times.dat

MS_EXEC="bin/merge_sort_exec"
QS_EXEC="bin/quick_sort_exec"
MEM_LIMIT="16777216"

# Loop through the 3 input files
for i in {1..3}
do
    INPUT_FILE="data/input_${i}.txt"
    MS_OUT_FILE="data/sorted_ms_${i}.txt"
    QS_OUT_FILE="data/sorted_qs_${i}.txt"

    echo "--- Processing file ${i}: ${INPUT_FILE} ---"

    # Time the merge sort
    echo "  Timing External Merge Sort..."
    ms_time=$( (time ${MS_EXEC} ${INPUT_FILE} ${MS_OUT_FILE} ${MEM_LIMIT} >/dev/null) 2>&1 )
    if [ $? -ne 0 ]; then
        echo "  Merge Sort command failed!"
        ms_time="-1.0"
    fi
    echo "  Merge Sort Time: $ms_time seconds"

    # Time the quick sort with a 5-minute timeout
    echo "  Timing External Quick Sort (with 5-minute timeout)..."
    
    # Use a subshell with timeout to ensure 'time' can be found
    qs_time=$( (timeout 300s bash -c "time ${QS_EXEC} ${INPUT_FILE} ${QS_OUT_FILE} ${MEM_LIMIT} >/dev/null" ) 2>&1 )
    qs_exit_code=$?

    if [ $qs_exit_code -eq 124 ]; then
        echo "  Quick Sort Timed Out!"
        qs_time="-1.0"
    elif [ $qs_exit_code -ne 0 ]; then
        echo "  Quick Sort command failed!"
        qs_time="-1.0"
    fi
    echo "  Quick Sort Time: $qs_time seconds"

    # Append results to the data file
    echo "${i},${ms_time},${qs_time}" >> report/times.dat

done

echo "--- Timing complete. Results are in report/times.dat ---"
