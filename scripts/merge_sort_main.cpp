#include <iostream>

void externalMergeSort(std::string inputFile, std::string outputFile, size_t memLimit) {
    std::cout << "External merge sort is not yet implemented.\n";
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: ./merge_sort_exec <input_file> <output_file> <memory_limit_bytes>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    size_t memLimit = std::stoull(argv[3]);

    externalMergeSort(inputFile, outputFile, memLimit);

    return 0;
}
