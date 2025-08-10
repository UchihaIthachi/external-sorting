#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <map>
#include <numeric>

// A map from Config Name (e.g., "K=4", "QS_A") to a vector of times for each run.
using ResultsMap = std::map<std::string, std::vector<double>>;

void print_table_rows(const std::string& algo_name, const std::vector<std::string>& config_order, const ResultsMap& results, size_t num_runs) {
    bool first_row = true;
    for (const auto& config : config_order) {
        if (results.find(config) == results.end()) continue;

        if (first_row) {
            // Calculate the number of configurations that will be printed for this algorithm
            size_t printed_configs = 0;
            for(const auto& c : config_order) {
                if(results.count(c)) {
                    printed_configs++;
                }
            }
            std::cout << "\\multirow{" << printed_configs << "}{*}{" << algo_name << "} ";
            first_row = false;
        }
        
        // Sanitize config name for LaTeX
        std::string sanitized_config = config;
        size_t pos = sanitized_config.find('_');
        if(pos != std::string::npos) {
            sanitized_config.replace(pos, 1, " ");
        }
        
        std::cout << "& " << sanitized_config << " ";

        const auto& times = results.at(config);
        double total_time = 0;
        int count = 0;
        for (double time : times) {
            if (time >= 0) {
                std::cout << "& " << std::fixed << std::setprecision(2) << time << " ";
                total_time += time;
                count++;
            } else {
                std::cout << "& FAILED ";
            }
        }
        // Pad empty cells if some runs are missing
        for(size_t i = times.size(); i < num_runs; ++i) {
            std::cout << "& --- ";
        }

        if (count > 0) {
            std::cout << "& \\textbf{" << std::fixed << std::setprecision(2) << total_time / count << "} \\\\\n";
        } else {
            std::cout << "& --- \\\\\n";
        }
    }
}

void print_latex_report(const ResultsMap& ms_results, const ResultsMap& qs_results, size_t num_runs) {
    std::cout << "\\documentclass{article}\n"
              << "\\usepackage{graphicx}\n"
              << "\\usepackage{booktabs}\n"
              << "\\usepackage{geometry}\n"
              << "\\usepackage{multirow}\n"
              << "\\usepackage[hidelinks]{hyperref}\n"
              << "\\usepackage{amsmath}\n"
              << "\\geometry{a4paper, margin=1in}\n"
              << "\\title{External Sorting Performance Report}\n"
              << "\\author{Fernando T.H.L. (210167E)}\n"
              << "\\date{\\today}\n"
              << "\\begin{document}\n"
              << "\\maketitle\n\n"

              // Overview
              << "\\section*{Overview}\n"
              << "This report presents a performance analysis of two external sorting algorithms: "
              << "\\emph{External Merge Sort} and \\emph{External Quick Sort}. "
              << "Each algorithm was evaluated under multiple configurations to study how the merge degree "
              << "(for merge sort) and buffer partitioning (for quick sort) affect runtime.\n\n"

              // Dataset
              << "\\subsection*{Dataset}\n"
              << "Unless otherwise noted, each input file is 256~MB and consists of \\textbf{67{,}108{,}864} integers "
              << "drawn from the range [1, 1{,}000{,}000]. We executed three independent runs per configuration.\n\n"

              // Heuristic K explanation
              << "\\subsection*{Heuristic for $K$ (Merge Degree)}\n"
              << "When no fixed $K$ is supplied, the implementation chooses $K$ using the heuristic:\n"
              << "\\[ K_{\\text{heuristic}} = \\min\\!\\left(8, \\left\\lfloor \\frac{\\texttt{memLimit}}{\\texttt{BUF\\_SIZE}\\times 2} \\right\\rfloor \\right). \\]\n"
              << "Intuition: with a total memory budget (\\texttt{memLimit}) and per-buffer size (\\texttt{BUF\\_SIZE}), the term "
              << "$\\left\\lfloor \\tfrac{\\texttt{memLimit}}{\\texttt{BUF\\_SIZE}\\times 2} \\right\\rfloor$ approximates how many input buffers can be "
              << "sustained while reserving comparable space for output and auxiliary structures; the cap at $8$ avoids overly small "
              << "per-run buffers and excessive I/O fragmentation. For example, with \\texttt{memLimit}=16~MB and \\texttt{BUF\\_SIZE}=1~MB, "
              << "$\\left\\lfloor \\tfrac{16}{1\\times 2} \\right\\rfloor = 8$, so the heuristic also yields $K=8$.\n\n"

              // Cross-check note
              << "\\subsection*{Cross-check with Shared Reports}\n"
              << "Two shared reports were consulted as reference points for file size and configuration conventions. "
              << "One states 256~MB files contain 67{,}108{,}864 integers, while another lists 44{,}739{,}242 integers. "
              << "Our experiments and this report use the \\textbf{67{,}108{,}864} value consistently. "
              << "Minor numeric differences likely arise from file generation scripts or formatting; "
              << "they do not affect the methodology presented here.\n\n"

              // Results table
              << "\\section*{Experimental Results}\n"
              << "Table~\\ref{tab:perf_summary} summarizes execution times for each algorithm and configuration across three runs.\n\n"
              << "\\begin{table}[h!]\n"
              << "\\centering\n"
              << "\\begin{tabular}{l l c c c c}\n"
              << "\\toprule\n"
              << "\\textbf{Algorithm} & \\textbf{Configuration} & \\textbf{Run 1 (s)} & \\textbf{Run 2 (s)} & \\textbf{Run 3 (s)} & \\textbf{Average (s)} \\\\\n"
              << "\\midrule\n";

    const std::vector<std::string> ms_config_order = {"K=heuristic", "K=4", "K=8", "K=16"};
    print_table_rows("Merge Sort", ms_config_order, ms_results, num_runs);

    std::cout << "\\midrule\n";

    const std::vector<std::string> qs_config_order = {"QS_A", "QS_B", "QS_C"};
    print_table_rows("Quick Sort", qs_config_order, qs_results, num_runs);

    std::cout << "\\bottomrule\n"
              << "\\end{tabular}\n"
              << "\\caption{Execution times (seconds) for external merge sort and external quick sort across three 256~MB input files. "
              << "Merge Sort uses \\(K\\in\\{\\text{heuristic},4,8,16\\}\\); Quick Sort uses three buffer splits: "
              << "QS\\_A: Input=2, Small=2, Large=2, Middle=10; "
              << "QS\\_B: Input=1, Small=1, Large=1, Middle=13; "
              << "QS\\_C: Input=2, Small=1, Large=1, Middle=12.}\n"
              << "\\label{tab:perf_summary}\n"
              << "\\end{table}\n\n"

              // Figures
              << "\\section*{Performance Analysis}\n"
              << "The figures below visualize the performance trends.\n\n"

              << "\\begin{figure}[h!]\n"
              << "\\centering\n"
              << "\\includegraphics[width=\\textwidth]{figures/merge_sort_time.png}\n"
              << "\\caption{External Merge Sort runtime by merge degree $K$ (heuristic, 4, 8, 16) for the three 256~MB inputs.}\n"
              << "\\label{fig:merge_sort_k_comparison}\n"
              << "\\end{figure}\n\n"

              << "\\begin{figure}[h!]\n"
              << "\\centering\n"
              << "\\includegraphics[width=\\textwidth]{figures/quick_sort_time.png}\n"
              << "\\caption{External Quick Sort runtime across buffer split configurations: "
              << "QS\\_A (2,2,2,10), QS\\_B (1,1,1,13), QS\\_C (2,1,1,12).}\n"
              << "\\label{fig:quick_sort_config_comparison}\n"
              << "\\end{figure}\n\n"

              << "\\begin{figure}[h!]\n"
              << "\\centering\n"
              << "\\includegraphics[width=0.8\\textwidth]{figures/best_vs_best.png}\n"
              << "\\caption{Best Merge Sort vs. Best Quick Sort per run (lowest time per algorithm per run).}\n"
              << "\\label{fig:best_vs_best}\n"
              << "\\end{figure}\n\n"

              // Summary
              << "\\section*{Summary}\n"
              << "Varying $K$ in merge sort and adjusting the Input/Small/Large/Middle page splits in quick sort "
              << "both have significant impact on execution time. Larger $K$ typically reduces the number of merge passes, "
              << "while quick sort benefits from allocating a sufficiently large Middle buffer for the in-memory pivot window. "
              << "The best-vs-best plot highlights the top performing configuration of each algorithm on the same inputs.\n"

              << "\\end{document}\n";
}


int main() {
    std::ifstream time_file("report/times.dat");
    if (!time_file) {
        std::cerr << "Error: Could not open report/times.dat.\n";
        return 1;
    }

    ResultsMap ms_results;
    ResultsMap qs_results;
    int max_run = 0;

    std::string line;
    std::getline(time_file, line); // Skip header

    while (std::getline(time_file, line)) {
        std::stringstream ss(line);
        std::string run_str, config, ms_time_str, qs_time_str;
        
        std::getline(ss, run_str, ',');
        std::getline(ss, config, ',');
        std::getline(ss, ms_time_str, ',');
        std::getline(ss, qs_time_str, ',');

        if (run_str.empty()) continue;
        
        int run_id = std::stoi(run_str);
        if(run_id > max_run) max_run = run_id;

        if (config.rfind("K=", 0) == 0) { // Starts with "K="
            try {
                ms_results[config].push_back(std::stod(ms_time_str));
            } catch (...) {
                ms_results[config].push_back(-1.0);
            }
        } else if (config.rfind("QS_", 0) == 0) { // Starts with "QS_"
            try {
                qs_results[config].push_back(std::stod(qs_time_str));
            } catch (...) {
                qs_results[config].push_back(-1.0);
            }
        }
    }
    time_file.close();

    if (ms_results.empty() && qs_results.empty()) {
        std::cerr << "Error: No data found in report/times.dat.\n";
        return 1;
    }

    print_latex_report(ms_results, qs_results, max_run);

    return 0;
}
