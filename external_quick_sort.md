# External Quick Sort Algorithm

This document provides a high-level overview and flowchart for the external quick sort algorithm implemented in this project.

## High-Level Explanation

The goal of this algorithm is to sort a large file (e.g., 256 MB) that does not fit into the available internal memory (e.g., 16 MB). The implementation uses a three-way partitioning strategy based on a specialized in-memory data structure, the **Interval Heap**.

The core idea is as follows:

1.  **In-Memory Pivot Selection**: A portion of the file is loaded into an in-memory interval heap. This heap efficiently maintains a "middle group" of elements and provides the minimum and maximum values of this group, which serve as dynamic pivots (`min_pivot` and `max_pivot`).

2.  **Three-Way Partitioning**: The algorithm streams through the rest of the file on disk. For each number, it compares it to the current pivots from the interval heap:

    - If the number is smaller than or equal to `min_pivot`, it is written to a "small" partition file on disk.
    - If the number is larger than or equal to `max_pivot`, it is written to a "large" partition file on disk.
    - If the number falls between the pivots, it belongs in the middle group. To make space, an element is evicted from the interval heap (either the min or max, depending on the strategy) and written to the appropriate "small" or "large" partition. The new number is then inserted into the heap.

3.  **Recursion**: After the entire input file is processed, the interval heap's contents are written to a "middle" partition file on disk. This middle partition is now sorted. The algorithm then recursively calls itself on the "small" and "large" partition files.

4.  **Base Case**: The recursion stops when a partition is small enough to fit entirely within the allocated internal memory. At this point, it is sorted in-memory using a standard sorting algorithm (like `std::sort`).

5.  **Final Merge**: Once the recursive calls return, the final sorted file is produced by simply concatenating the sorted "small" partition, the sorted "middle" partition, and the sorted "large" partition in that order.

This approach effectively breaks down the massive sorting problem into smaller, manageable chunks that can be processed recursively.

## Algorithm Flowchart

```mermaid
graph TD
    A[Start] --> B{File size <= Memory Limit?};
    B -- Yes --> C[Sort in Memory];
    C --> D[Write to Output File];
    D --> E[End];
    B -- No --> F[Load Initial Chunk into Interval Heap];
    F --> G[Determine Min/Max Pivots from Heap];
    G --> H[Stream Through Remaining Input File];
    H -- More data --> I{Partition Current Element};
    I -- Element <= Min Pivot --> J[Write to 'small' Partition];
    I -- Element >= Max Pivot --> K[Write to 'large' Partition];
    I -- Min Pivot < Element < Max Pivot --> L[Evict from Heap & Insert Element];
    J --> H;
    K --> H;
    L --> H;
    H -- End of File --> M[Write Heap Contents to 'middle' Partition];
    M --> N[Recursively Sort 'small' Partition];
    N --> O[Recursively Sort 'large' Partition];
    O --> P[Concatenate sorted 'small', 'middle', and sorted 'large' partitions];
    P --> D;
```

## Memory Layout

The 16 MB of available RAM is partitioned to accommodate the data structures and buffers needed for the sorting process. The layout is approximately as follows:

```mermaid
graph TD
    subgraph "16 MB Main Memory"
        direction LR
        A["Input Buffer <br>(e.g., 1MB)"]
        B["Output Buffers <br>(e.g., 2 x 1MB for 'small' and 'large' partitions)"]
        C["Interval Heap <br>(Remaining Memory, e.g., ~13MB)"]
    end
```

- **Input Buffer**: A buffer to read chunks of the input file from disk efficiently.
- **Output Buffers**: Two buffers are used to write to the "small" and "large" partition files on disk.
- **Interval Heap**: The largest portion of the memory is allocated to the interval heap, which holds the pivot elements. The size of the heap directly impacts the quality of the pivots and the number of elements that need to be re-written to the small/large partitions.
