#include "external_merge_sort.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <mem_limit_in_bytes>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    size_t memLimit = std::stoul(argv[3]);

    externalMergeSort(inputFile, outputFile, memLimit);

    std::cout << "External merge sort completed.\n";
    return 0;
}
