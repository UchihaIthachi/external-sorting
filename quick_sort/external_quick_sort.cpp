#include "external_quick_sort.hpp"
#include <iostream>

void externalQuickSort(std::string inputFile, std::string outputFile, size_t memLimit) {
    const size_t BUF_SIZE = 1 * 1024 * 1024;  // 1 MB per buffer
    IntervalHeap pivotHeap((memLimit - 3 * BUF_SIZE) / sizeof(int));

    std::ifstream in(inputFile, std::ios::binary);
    if (!in) {
        std::cerr << "Failed to open input file.\n";
        return;
    }

    in.seekg(0, std::ios::end);
    size_t fileSize = in.tellg();
    in.seekg(0);

    if (fileSize <= memLimit) {
        std::vector<int> data;
        int x;
        while (in.read((char*)&x, sizeof(int))) {
            data.push_back(x);
        }
        in.close();
        std::sort(data.begin(), data.end());
        std::ofstream out(outputFile, std::ios::binary);
        for (int val : data) {
            out.write((char*)&val, sizeof(int));
        }
        out.close();
        return;
    }

    std::ofstream smallOut("partition_small.bin", std::ios::binary);
    std::ofstream largeOut("partition_large.bin", std::ios::binary);

    int value;
    size_t loaded = 0;
    while (!pivotHeap.isFull() && in.read((char*)&value, sizeof(int))) {
        pivotHeap.insert(value);
        loaded++;
    }

    int minPivot = pivotHeap.getMin();
    int maxPivot = pivotHeap.getMax();

    while (in.read((char*)&value, sizeof(int))) {
        if (value <= minPivot) {
            smallOut.write((char*)&value, sizeof(int));
        } else if (value >= maxPivot) {
            largeOut.write((char*)&value, sizeof(int));
        } else {
            int evicted;
            if (value < ((long long)minPivot + maxPivot) / 2) {
                evicted = pivotHeap.removeMin();
                smallOut.write((char*)&evicted, sizeof(int));
            } else {
                evicted = pivotHeap.removeMax();
                largeOut.write((char*)&evicted, sizeof(int));
            }
            pivotHeap.insert(value);
        }
    }
    in.close();
    smallOut.close();
    largeOut.close();

    std::ofstream midOut("partition_middle.bin", std::ios::binary);
    while (!pivotHeap.isEmpty()) {
        int out = pivotHeap.removeMin();
        midOut.write((char*)&out, sizeof(int));
    }
    midOut.close();

    externalQuickSort("partition_small.bin", "sorted_small.bin", memLimit);
    externalQuickSort("partition_large.bin", "sorted_large.bin", memLimit);

    std::ifstream f1("sorted_small.bin", std::ios::binary);
    std::ifstream f2("partition_middle.bin", std::ios::binary);
    std::ifstream f3("sorted_large.bin", std::ios::binary);
    std::ofstream finalOut(outputFile, std::ios::binary);

    while (f1.read((char*)&value, sizeof(int)))
        finalOut.write((char*)&value, sizeof(int));
    while (f2.read((char*)&value, sizeof(int)))
        finalOut.write((char*)&value, sizeof(int));
    while (f3.read((char*)&value, sizeof(int)))
        finalOut.write((char*)&value, sizeof(int));

    f1.close();
    f2.close();
    f3.close();
    finalOut.close();
}
