#include "external_quick_sort.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: ./quick_sort <input_file> <output_file> <mem_limit_bytes>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    size_t memLimit = std::stoul(argv[3]);

    externalQuickSort(inputFile, outputFile, memLimit);

    std::cout << "Sorting completed!\n";
    return 0;
}
