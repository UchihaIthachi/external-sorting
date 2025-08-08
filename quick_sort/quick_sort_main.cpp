#include "external_quick_sort.hpp"
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
        memLimit = std::stoull(args[2]);
    } else {
        std::cerr << "Usage: ./quick_sort_exec <input_file> <output_file> <memory_limit_bytes> [--verbose]\n";
        return 1;
    }

    if (args.size() == 4 && args[3] == "--verbose") {
        verbose = true;
    }

    g_debug_logging_enabled = verbose;

    if (g_debug_logging_enabled) {
        std::cerr << "Debug logging enabled." << std::endl;
    }

    externalQuickSort(inputFile, outputFile, memLimit, 0);

    return 0;
}
