# === Configurable Variables ===
REPO_USER = UchihaIthachi
REPO_NAME = external-sorting
BRANCH    = main
REMOTE_TEX = https://raw.githubusercontent.com/$(REPO_USER)/$(REPO_NAME)/$(BRANCH)/report/main.tex

# === Compiler Setup ===
CXX = g++
CXXFLAGS = -std=c++17 -O2

# === Output Folder ===
BIN_DIR = bin
$(shell mkdir -p $(BIN_DIR))

# === Source Files ===
QS_SRC = quick_sort/quick_sort_main.cpp \
         quick_sort/external_quick_sort.cpp \
         quick_sort/interval_heap.cpp

MS_SRC = merge_sort/merge_sort_main.cpp \
         merge_sort/external_merge_sort.cpp \
         merge_sort/tournament_tree.cpp \
         merge_sort/huffman_merge.cpp \
         merge_sort/io_utils.cpp

GEN_SRC = scripts/generate_input.cpp
CMP_SRC = scripts/compare_output.cpp
VS_SRC = scripts/verify_sorted.cpp

# === Binaries ===
QS_OUT = $(BIN_DIR)/quick_sort_exec
MS_OUT = $(BIN_DIR)/merge_sort_exec
GEN_OUT = $(BIN_DIR)/generate_input
CMP_OUT = $(BIN_DIR)/compare_output
VS_OUT = $(BIN_DIR)/verify_sorted

# === Default: Build Everything ===
all: $(QS_OUT) $(MS_OUT) $(GEN_OUT) $(CMP_OUT) $(VS_OUT)

# === Targets ===
$(QS_OUT): $(QS_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Built: $@"

$(MS_OUT): $(MS_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Built: $@"

$(GEN_OUT): $(GEN_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Built: $@"

$(CMP_OUT): $(CMP_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Built: $@"

# === Run Targets ===
# These can be overridden from the command line, e.g., make run-ms INPUT_FILE=...
INPUT_FILE ?= data/input_1.txt
QS_OUTPUT_FILE ?= data/sorted_qs_1.txt
MS_OUTPUT_FILE ?= data/sorted_ms_1.txt
MEM_LIMIT ?= 16777216

run-qs: $(QS_OUT)
	@$(QS_OUT) $(INPUT_FILE) $(QS_OUTPUT_FILE) $(MEM_LIMIT)

run-ms: $(MS_OUT)
	@$(MS_OUT) $(INPUT_FILE) $(MS_OUTPUT_FILE) $(MEM_LIMIT)

run-all: run-qs run-ms

# === Input Generation ===
GEN_OUT = $(BIN_DIR)/generate_input

.PHONY: generate generate-3-files
generate: generate-3-files

generate-3-files: $(GEN_OUT)
	@echo "--- Generating 3 input files (256MB each) ---"
	@$(GEN_OUT) data/input_1.txt 256
	@$(GEN_OUT) data/input_2.txt 256
	@$(GEN_OUT) data/input_3.txt 256
	@echo "--- Input file generation complete ---"

# === Cleanup ===
clean:
	rm -rf $(BIN_DIR)
	@echo "🧹 Cleaned all binaries"

clean-partitions:
	rm -f partition_*.bin sorted_*.bin

# === Build Each Separately ===
quick_sort: $(QS_OUT)
merge_sort: $(MS_OUT)
scripts: $(GEN_OUT)

$(VS_OUT): $(VS_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Built: $@"

# === Verification Targets ===
verify-qs: $(QS_OUT) $(VS_OUT) $(GEN_OUT)
	@echo "--- Verifying Quick Sort ---"
	@$(GEN_OUT) data/input_1.txt 256
	@$(QS_OUT) data/input_1.txt data/sorted_qs_1.txt 16777216 2> stderr.log
	@if ! $(VS_OUT) data/sorted_qs_1.txt; then \
		echo "Verification failed: File is not sorted."; \
		exit 1; \
	else \
		echo "Verification successful: File is sorted."; \
	fi
	@echo "----------------------------"

verify-ms: $(MS_OUT) $(VS_OUT)
	@echo "--- Verifying Merge Sort ---"
	@$(MS_OUT) data/input_1.txt data/sorted_ms_1.txt 16777216
	@$(VS_OUT) data/sorted_ms_1.txt
	@echo "----------------------------"

# === Report Generation ===
REPORT_SRC = scripts/generate_report.cpp
REPORT_OUT = $(BIN_DIR)/generate_report
TIME_SCRIPT = scripts/run_and_time.sh
PLOT_SCRIPT = scripts/plot_merge_sort_time.py
TEX_OUT = report/report.tex

$(REPORT_OUT): $(REPORT_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Built: $@"

report: $(REPORT_OUT) generate-3-files
	@echo "--- Running experiments and generating report ---"
	@bash $(TIME_SCRIPT)
	@python $(PLOT_SCRIPT)
	@$(REPORT_OUT) > $(TEX_OUT)
	@echo "--- Report generation complete: $(TEX_OUT) ---"

pdf: report/report.tex
	@echo "--- Compiling LaTeX to PDF via online service ---"
	@curl -fLs "https://latexonline.cc/compile?url=$(REMOTE_TEX)" -o report/report.pdf \
	 || { echo "latexonline failed. Is the repo public and the file/URL correct?"; exit 1; }
	@echo "PDF saved to report/report.pdf"

# === Declare Phony Targets ===
.PHONY: all clean clean-partitions quick_sort merge_sort scripts \
        run-qs run-ms run-all generate-3-files verify-qs verify-ms report pdf
