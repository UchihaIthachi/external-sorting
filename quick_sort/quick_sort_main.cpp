#include "external_quick_sort.hpp"
#include "logger.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // For std::find

// Define the global logger flag
bool g_debug_logging_enabled = false;

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);
    bool verbose = false;

    auto verbose_it = std::find(args.begin(), args.end(), "--verbose");
    if (verbose_it != args.end()) {
        verbose = true;
        args.erase(verbose_it);
    }
    g_debug_logging_enabled = verbose;
    if (g_debug_logging_enabled) {
        std::cerr << "Debug logging enabled." << std::endl;
    }
    
    // Simple command-line parsing
    std::string inputFile, outputFile;
    size_t memLimit = 0;

    if (args.size() != 3 && args.size() != 7) {
        std::cerr << "Usage: ./quick_sort_exec <input_file> <output_file> <memory_limit_bytes> [in_mb small_mb large_mb middle_mb] [--verbose]\n";
        return 1;
    }

    inputFile = args[0];
    outputFile = args[1];
    memLimit = std::stoull(args[2]);

    if (args.size() == 7) {
        try {
            int in_mb = std::stoi(args[3]);
            int small_mb = std::stoi(args[4]);
            int large_mb = std::stoi(args[5]);
            int middle_mb = std::stoi(args[6]);
            externalQuickSort(inputFile, outputFile, memLimit, 0, in_mb, small_mb, large_mb, middle_mb);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid buffer/heap size argument. All four must be integers." << std::endl;
            return 1;
        }
    } else {
        // Call with default buffer/heap sizes
        externalQuickSort(inputFile, outputFile, memLimit, 0);
    }

    return 0;
}
