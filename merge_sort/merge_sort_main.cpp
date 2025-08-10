#include "external_merge_sort.hpp"
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

    // Check for --verbose flag and remove it from args
    auto verbose_it = std::find(args.begin(), args.end(), "--verbose");
    if (verbose_it != args.end()) {
        verbose = true;
        args.erase(verbose_it);
    }

    g_debug_logging_enabled = verbose;
    if (g_debug_logging_enabled) {
        std::cerr << "Debug logging enabled for merge sort." << std::endl;
    }

    // Command-line parsing
    std::string inputFile, outputFile;
    size_t memLimit = 0;
    int k_val = 0; // Default to 0 for heuristic

    if (args.size() < 3 || args.size() > 4) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <mem_limit_in_bytes> [K_value] [--verbose]\n";
        return 1;
    }

    inputFile = args[0];
    outputFile = args[1];
    memLimit = std::stoul(args[2]);

    if (args.size() == 4) {
        try {
            k_val = std::stoi(args[3]);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid K value: '" << args[3] << "'. Must be an integer." << std::endl;
            std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <mem_limit_in_bytes> [K_value] [--verbose]\n";
            return 1;
        }
    }

    externalMergeSort(inputFile, outputFile, memLimit, k_val);

    std::cout << "External merge sort completed.\n";
    return 0;
}
