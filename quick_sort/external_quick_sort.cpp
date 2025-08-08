#include "external_quick_sort.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

void externalQuickSort(std::string inputFile, std::string outputFile, size_t memLimit, int recursion_level) {
    std::cout << "Recursion level: " << recursion_level << std::endl;
    std::cout << "Input file: " << inputFile << std::endl;
    std::cout << "Output file: " << outputFile << std::endl;

    const size_t BUF_SIZE = 1 * 1024 * 1024;  // 1 MB buffer
    if (memLimit <= 3 * BUF_SIZE) {
        std::cerr << "Memory limit too small\n";
        return;
    }
    IntervalHeap pivotHeap((memLimit - 3 * BUF_SIZE) / sizeof(int));  // Your fixed interval heap here

    std::ifstream in(inputFile, std::ios::binary);
    if (!in) {
        std::cerr << "Failed to open input file: " << inputFile << "\n";
        return;
    }

    in.seekg(0, std::ios::end);
    size_t fileSize = in.tellg();
    in.seekg(0);
    std::cout << "File size: " << fileSize << " bytes" << std::endl;

    if (fileSize <= memLimit) {
        std::cout << "File is small enough to sort in memory." << std::endl;
        std::vector<int> data;
        int x;
        while (in.read((char*)&x, sizeof(int))) data.push_back(x);
        in.close();
        std::sort(data.begin(), data.end());
        std::ofstream out(outputFile, std::ios::binary);
        for (int val : data) out.write((char*)&val, sizeof(int));
        out.close();
        return;
    }

    std::stringstream smallName, largeName, middleName, sortedSmallName, sortedLargeName;
    smallName << "partition_small_" << recursion_level << ".bin";
    largeName << "partition_large_" << recursion_level << ".bin";
    middleName << "partition_middle_" << recursion_level << ".bin";
    sortedSmallName << "sorted_small_" << recursion_level << ".bin";
    sortedLargeName << "sorted_large_" << recursion_level << ".bin";

    std::ofstream smallOut(smallName.str(), std::ios::binary);
    std::ofstream largeOut(largeName.str(), std::ios::binary);

    int value;
    size_t loaded = 0;
    std::cout << "Loading initial pivot heap..." << std::endl;
    while (!pivotHeap.isFull() && in.read((char*)&value, sizeof(int))) {
        pivotHeap.insert(value);
        loaded++;
    }
    std::cout << "Initial pivot heap loaded with " << loaded << " elements." << std::endl;

    int minPivot = pivotHeap.getMin();
    int maxPivot = pivotHeap.getMax();
    std::cout << "Pivots: " << minPivot << ", " << maxPivot << std::endl;

    if (minPivot > maxPivot) {
        std::cerr << "ERROR: minPivot > maxPivot, invalid heap state\n";
        return;
    }

    std::cout << "Partitioning file..." << std::endl;
    long long count = 0;
    int last_h_min = 0;
    bool violation_found = false;
    while (in.read((char*)&value, sizeof(int))) {
        int h_min = pivotHeap.getMin();
        int h_max = pivotHeap.getMax();

        if (!violation_found && h_min < last_h_min) {
            std::cerr << "!!! VIOLATION: h_min decreased! " << last_h_min << " -> " << h_min << " at item " << count << std::endl;
            violation_found = true;
        }
        last_h_min = h_min;

        bool should_log = (count >= 611330 && count <= 611340) || (violation_found && count < 611340);

        if (should_log) {
             std::cerr << "i=" << count << " | val=" << value
                      << " | h_min=" << h_min << ", h_max=" << h_max;
        }


        if (value <= h_min) {
            if (should_log) std::cerr << " -> small" << std::endl;
            smallOut.write((char*)&value, sizeof(int));
        } else if (value >= h_max) {
            if (should_log) std::cerr << " -> large" << std::endl;
            largeOut.write((char*)&value, sizeof(int));
        } else {
            int evicted;
            if (value < ((long long)h_min + h_max) / 2) {
                if (should_log) std::cerr << " | evict min path...";
                evicted = pivotHeap.removeMin();
                if (should_log) std::cerr << " evicted=" << evicted << ", insert " << value << std::endl;
                smallOut.write((char*)&evicted, sizeof(int));
            } else {
                if (should_log) std::cerr << " | evict max path...";
                evicted = pivotHeap.removeMax();
                if (should_log) std::cerr << " evicted=" << evicted << ", insert " << value << std::endl;
                largeOut.write((char*)&evicted, sizeof(int));
            }
            pivotHeap.insert(value);
        }
        count++;
    }
    in.close();
    smallOut.close();
    largeOut.close();

    std::ofstream midOut(middleName.str(), std::ios::binary);
    std::cout << "Writing middle partition..." << std::endl;
    while (!pivotHeap.isEmpty()) {
        int out = pivotHeap.removeMin();
        midOut.write((char*)&out, sizeof(int));
    }
    midOut.close();
    std::cout << "Middle partition written." << std::endl;

    std::cout << "Recursive call for small partition." << std::endl;
    externalQuickSort(smallName.str(), sortedSmallName.str(), memLimit, recursion_level + 1);
    std::cout << "Recursive call for large partition." << std::endl;
    externalQuickSort(largeName.str(), sortedLargeName.str(), memLimit, recursion_level + 1);

    std::cout << "Merging partitions..." << std::endl;
    std::ifstream f1(sortedSmallName.str(), std::ios::binary);
    std::ifstream f2(middleName.str(), std::ios::binary);
    std::ifstream f3(sortedLargeName.str(), std::ios::binary);
    std::ofstream finalOut(outputFile, std::ios::binary);

    while (f1.read((char*)&value, sizeof(int))) finalOut.write((char*)&value, sizeof(int));
    while (f2.read((char*)&value, sizeof(int))) finalOut.write((char*)&value, sizeof(int));
    while (f3.read((char*)&value, sizeof(int))) finalOut.write((char*)&value, sizeof(int));

    f1.close();
    f2.close();
    f3.close();
    finalOut.close();
    std::cout << "Partitions merged." << std::endl;
}
