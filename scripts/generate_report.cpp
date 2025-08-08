#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <numeric>

struct RunResult {
    int run_id;
    double time_ms;
    double time_qs;
};

void print_latex_report(const std::vector<RunResult>& results) {
    // Preamble
    std::cout << "\\documentclass{article}\n";
    std::cout << "\\usepackage{graphicx}\n";
    std::cout << "\\usepackage{booktabs}\n";
    std::cout << "\\usepackage{geometry}\n";
    std::cout << "\\geometry{a4paper, margin=1in}\n";
    std::cout << "\\title{External Sorting Performance Report}\n";
    std::cout << "\\author{}\n";
    std::cout << "\\date{\\today}\n";
    std::cout << "\\begin{document}\n";
    std::cout << "\\maketitle\n\n";

    // Overview Section
    std::cout << "\\section*{Overview}\n";
    std::cout << "We implemented and evaluated two external sorting algorithms over three separate 256 MB input files, with a 16 MB memory limit.\n\n";

    // Results Table
    std::cout << "\\subsection*{Performance Results}\n";
    std::cout << "\\begin{table}[h!]\n";
    std::cout << "\\centering\n";
    std::cout << "\\begin{tabular}{l c c}\n";
    std::cout << "\\toprule\n";
    std::cout << "Test Run & Merge Sort Time (s) & Quick Sort Time (s) \\\\\n";
    std::cout << "\\midrule\n";

    double ms_total_time = 0;
    int ms_successful_runs = 0;

    for (const auto& res : results) {
        std::cout << "Run " << res.run_id << " & ";
        if (res.time_ms < 0) {
            std::cout << "FAILED & ";
        } else {
            std::cout << std::fixed << std::setprecision(2) << res.time_ms << " & ";
            ms_total_time += res.time_ms;
            ms_successful_runs++;
        }
        if (res.time_qs < 0) {
            std::cout << "Timed Out \\\\\n";
        } else {
            std::cout << std::fixed << std::setprecision(2) << res.time_qs << " \\\\\n";
        }
    }
    
    std::cout << "\\midrule\n";
    // Average row
    if (ms_successful_runs > 0) {
        double avg_ms_time = ms_total_time / ms_successful_runs;
        std::cout << "\\textbf{Average} & \\textbf{" << std::fixed << std::setprecision(2) << avg_ms_time << "} & \\textbf{---} \\\\\n";
    } else {
        std::cout << "\\textbf{Average} & \\textbf{FAILED} & \\textbf{---} \\\\\n";
    }

    std::cout << "\\bottomrule\n";
    std::cout << "\\end{tabular}\n";
    std::cout << "\\caption{Performance summary of external sorting algorithms across 3 runs.}\n";
    std::cout << "\\label{tab:perf_summary}\n";
    std::cout << "\\end{table}\n\n";

    // Figure
    std::cout << "\\subsection*{Figure: Merge Sort Performance}\n";
    std::cout << "\\begin{figure}[h!]\n";
    std::cout << "\\centering\n";
    std::cout << "\\includegraphics[width=0.8\\textwidth]{figures/merge_sort_time.png}\n";
    std::cout << "\\caption{Execution time for external merge sort on three different 256 MB files.}\n";
    std::cout << "\\label{fig:merge_sort_time}\n";
    std::cout << "\\end{figure}\n\n";
    
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
        
        std::getline(ss, part, ',');
        res.run_id = std::stoi(part);
        
        std::getline(ss, part, ',');
        res.time_ms = std::stod(part);

        std::getline(ss, part, ',');
        res.time_qs = std::stod(part);

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
