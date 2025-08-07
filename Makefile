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

# === Run Targets (only one file) ===
run-qs: $(QS_OUT)
	@$(QS_OUT) data/input_1.txt data/sorted_qs_1.txt 16777216

run-ms: $(MS_OUT)
	@$(MS_OUT) data/input_1.txt data/sorted_ms_1.txt 16777216

run-all: run-qs run-ms

# === Input Generation (only one file) ===
generate: $(GEN_OUT)
	@$(GEN_OUT) data/input_1.txt 32  # decrease for debugging purpose 
	@echo "Generated 1 × 256MB input file"

# === Cleanup ===
clean:
	rm -rf $(BIN_DIR)
	@echo "🧹 Cleaned all binaries"

clean-partitions:
	rm -f partition_*.bin sorted_*.bin

# === Build Each Separately ===
quick_sort: $(QS_OUT)
merge_sort: $(MS_OUT)
scripts: $(GEN_OUT) $(CMP_OUT)

$(VS_OUT): $(VS_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Built: $@"

# === Verification Targets ===
verify-qs: $(QS_OUT) $(VS_OUT)
	@echo "--- Verifying Quick Sort ---"
	@$(QS_OUT) data/input_1.txt data/sorted_qs_1.txt 16777216
	@$(VS_OUT) data/sorted_qs_1.txt
	@echo "----------------------------"

verify-ms: $(MS_OUT) $(VS_OUT)
	@echo "--- Verifying Merge Sort ---"
	@$(MS_OUT) data/input_1.txt data/sorted_ms_1.txt 16777216
	@$(VS_OUT) data/sorted_ms_1.txt
	@echo "----------------------------"

# === Declare Phony Targets ===
.PHONY: all clean clean-partitions quick_sort merge_sort scripts \
        run-qs run-ms run-all generate-256 verify-qs verify-ms
