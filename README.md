# External Sorting

This project implements external sorting using quick sort and merge sort.

## Algorithm Explanations

For a detailed explanation of the external quick sort algorithm, including a flowchart of the process, please see [external_quick_sort.md](external_quick_sort.md).

For a detailed explanation of the external merge sort algorithm, including its two-phase process, please see [external_merge_sort.md](external_merge_sort.md).

## Building the Code

To build the code, run the following command:

```
make
```

This will create the executables in the `bin` directory.

## Running the Code

To run the quick sort algorithm on a 256MB file, run the following commands:

```
make generate
make run-qs
```

To run the merge sort algorithm, you can use the `run-ms` target:

```
make run-ms
```

## Cleaning up

To clean up the build files, run:

```
make clean
```

To clean up the temporary partition files, run:

```
make clean-partitions
```

## Benchmark and Record Runtimes

You can wrap each run in /usr/bin/time -v or use Bash timing:

```
time make run-qs
time make run-ms
```
