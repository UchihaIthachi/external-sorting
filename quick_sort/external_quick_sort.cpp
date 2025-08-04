#include "external_quick_sort.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

void externalQuickSort(std::string inputFile, std::string outputFile, size_t memLimit, int recursion_level) {
    std::cout << "Recursion level: " << recursion_level << std::endl;
    std::cout << "Input file: " << inputFile << std::endl;
    std::cout << "Output file: " << outputFile << std::endl;

    const size_t BUF_SIZE = 1 * 1024 * 1024;  // 1 MB per buffer
    if (memLimit <= 3 * BUF_SIZE) {
        std::cerr << "Memory limit is too small.\n";
        return;
    }
    IntervalHeap pivotHeap((memLimit - 3 * BUF_SIZE) / sizeof(int));

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

    std::stringstream smallName, largeName, middleName, sortedSmallName, sortedLargeName;
    smallName << "partition_small_" << recursion_level << ".bin";
    largeName << "partition_large_" << recursion_level << ".bin";
    middleName << "partition_middle_" << recursion_level << ".bin";
    sortedSmallName << "sorted_small_" << recursion_level << ".bin";
    sortedLargeName << "sorted_large_" << recursion_level << ".bin";

    std::cout << "Small partition: " << smallName.str() << std::endl;
    std::cout << "Large partition: " << largeName.str() << std::endl;
    std::cout << "Middle partition: " << middleName.str() << std::endl;


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

    std::cout << "Getting pivots..." << std::endl;
    int minPivot = pivotHeap.getMin();
    int maxPivot = pivotHeap.getMax();
    std::cout << "Pivots: " << minPivot << ", " << maxPivot << std::endl;

    std::cout << "Partitioning file..." << std::endl;
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
    std::cout << "File partitioned." << std::endl;

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
    std::cout << "Partitions merged." << std::endl;
}
