#include "external_merge_sort.hpp"
#include "logger.hpp"
#include <iostream>
#include <string>
#include <vector>

// Define the global logger flag
bool g_debug_logging_enabled = false;

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);
    bool verbose = false;

    // Simple command-line parsing
    std::string inputFile, outputFile;
    size_t memLimit = 0;

    if (args.size() >= 3) {
        inputFile = args[0];
        outputFile = args[1];
        memLimit = std::stoul(args[2]);
    } else {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <mem_limit_in_bytes> [--verbose]\n";
        return 1;
    }

    if (args.size() == 4 && args[3] == "--verbose") {
        verbose = true;
    }

    g_debug_logging_enabled = verbose;

    if (g_debug_logging_enabled) {
        std::cerr << "Debug logging enabled for merge sort." << std::endl;
    }

    externalMergeSort(inputFile, outputFile, memLimit);

    std::cout << "External merge sort completed.\n";
    return 0;
}
