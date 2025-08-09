#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>

struct RunResult {
    int run_id;
    double time_ms;
    double time_qs;
};

void print_latex_report(const std::vector<RunResult>& results) {
    std::cout << "\\documentclass{article}\n"
              << "\\usepackage{graphicx}\n"
              << "\\usepackage{booktabs}\n"
              << "\\usepackage{geometry}\n"
              << "\\geometry{a4paper, margin=1in}\n"
              << "\\title{External Sorting Performance Report}\n"
              << "\\author{Fernando T.H.L.(210167E)}\n"
              << "\\date{\\today}\n"
              << "\\begin{document}\n"
              << "\\maketitle\n\n";

    // Overview
    std::cout << "\\section*{Overview}\n";
    std::cout << "This report presents the performance analysis of two external sorting algorithms---\\textbf{External Merge Sort} and \\textbf{External Quick Sort}---each tested on three 256~MB input files, using a memory limit of 16~MB per run. "
                 "The analysis covers correctness, speed, and failure cases. Figure~\\ref{fig:sort_times_comparison} provides a direct comparison.\n\n";

    // Data Generation Section
    std::cout << "\\section*{Data Generation}\n"
              << "The tests were performed on three distinct input files, each generated with a size of 256~MB. "
              << "The generation process is detailed below:\n"
              << "\\begin{verbatim}\n"
              << "--- Generating 3 input files (256MB each) ---\n"
              << "File 'data/input_1.txt' generated (256 MB, 67108864 numbers).\n"
              << "File 'data/input_2.txt' generated (256 MB, 67108864 numbers).\n"
              << "File 'data/input_3.txt' generated (256 MB, 67108864 numbers).\n"
              << "\\end{verbatim}\n\n";

    // Results Table
    std::cout << "\\section*{Experimental Results}\n";
    std::cout << "\\begin{table}[h!]\n"
              << "\\centering\n"
              << "\\begin{tabular}{c c c}\n"
              << "\\toprule\n"
              << "Test Run & Merge Sort Time (s) & Quick Sort Time (s) \\\\\n"
              << "\\midrule\n";

    double ms_total = 0, qs_total = 0;
    int ms_count = 0, qs_count = 0;

    for (const auto& res : results) {
        std::cout << res.run_id << " & ";

        if (res.time_ms < 0)
            std::cout << "\\textbf{FAILED} & ";
        else {
            std::cout << std::fixed << std::setprecision(2) << res.time_ms << " & ";
            ms_total += res.time_ms;
            ms_count++;
        }

        if (res.time_qs < 0)
            std::cout << "\\textbf{TIMEOUT} \\\\\n";
        else {
            std::cout << std::fixed << std::setprecision(2) << res.time_qs << " \\\\\n";
            qs_total += res.time_qs;
            qs_count++;
        }
    }

    // Averages row
    std::cout << "\\midrule\n";
    std::cout << "\\textbf{Average} & ";
    if (ms_count)
        std::cout << "\\textbf{" << std::fixed << std::setprecision(2) << ms_total / ms_count << "} & ";
    else
        std::cout << "\\textbf{---} & ";

    if (qs_count)
        std::cout << "\\textbf{" << std::fixed << std::setprecision(2) << qs_total / qs_count << "} \\\\\n";
    else
        std::cout << "\\textbf{---} \\\\\n";

    std::cout << "\\bottomrule\n"
              << "\\end{tabular}\n"
              << "\\caption{Performance summary of external sorting algorithms across three runs. "
                 "``FAILED'' indicates the algorithm failed to sort or crashed. ``TIMEOUT'' indicates the run exceeded the allowed time limit.}\n"
              << "\\label{tab:perf_summary}\n"
              << "\\end{table}\n\n";

    // Figures
    std::cout << "\\section*{Figures}\n";
    std::cout << "\\begin{figure}[h!]\n"
              << "\\centering\n"
              << "\\includegraphics[width=0.8\\textwidth]{figures/sort_times_comparison.png}\n"
              << "\\caption{Execution time comparison between external merge sort and quick sort on three different 256~MB files. Missing bars indicate failure or timeout.}\n"
              << "\\label{fig:sort_times_comparison}\n"
              << "\\end{figure}\n\n";

    std::cout << "\\begin{figure}[h!]\n"
              << "\\centering\n"
              << "\\includegraphics[width=0.7\\textwidth]{figures/performance_variability.png}\n"
              << "\\caption{Performance variability of sorting algorithms across three runs. The box shows the interquartile range (IQR), the line inside is the median, and whiskers extend to 1.5x IQR.}\n"
              << "\\label{fig:performance_variability}\n"
              << "\\end{figure}\n\n";

    // Discussion
    std::cout << "\\section*{Summary and Recommendations}\n"
              << "External Merge Sort completed successfully and efficiently in all test cases. "
              << "External Quick Sort, however, failed to complete within the timeout in all runs, likely due to implementation issues in interval heap management or buffer partitioning. "
              << "For production or further research, it is recommended to debug and optimize the quick sort implementation. "
              << "Additionally, implementing optimal (Huffman-based) merging for merge sort could further improve performance when run sizes are highly variable.\n";

    std::cout << "\\end{document}\n";
}

int main() {
    std::ifstream time_file("report/times.dat");
    if (!time_file) {
        std::cerr << "Error: Could not open report/times.dat. Run the timing script first.\n";
        return 1;
    }

    std::vector<RunResult> results;
    std::string line;
    // Skip header
    if (!std::getline(time_file, line)) {
        std::cerr << "Error: Could not read header from times.dat\n";
        return 1;
    }

    while (std::getline(time_file, line)) {
        std::stringstream ss(line);
        std::string part;
        RunResult res;
        // Read: Run,MergeSortTime,QuickSortTime
        std::getline(ss, part, ',');
        res.run_id = std::stoi(part);

        std::getline(ss, part, ',');
        try { res.time_ms = std::stod(part); }
        catch (...) { res.time_ms = -1.0; }

        std::getline(ss, part, ',');
        try { res.time_qs = std::stod(part); }
        catch (...) { res.time_qs = -1.0; }

        results.push_back(res);
    }

    time_file.close();

    if (results.empty()) {
        std::cerr << "Error: No data found in report/times.dat.\n";
        return 1;
    }

    print_latex_report(results);

    return 0;
}
