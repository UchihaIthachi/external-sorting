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
         merge_sort/tournament_tree.cpp \
         merge_sort/huffman_merge.cpp

GEN_SRC = scripts/generate_input.cpp
CMP_SRC = scripts/compare_output.cpp

# === Binaries ===
QS_OUT = $(BIN_DIR)/quick_sort_exec
MS_OUT = $(BIN_DIR)/merge_sort_exec
GEN_OUT = $(BIN_DIR)/generate_input
CMP_OUT = $(BIN_DIR)/compare_output

# === Default: Build Everything ===
all: $(QS_OUT) $(MS_OUT) $(GEN_OUT) $(CMP_OUT)

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
run-qs: $(QS_OUT)
	@$(QS_OUT) data/input_1.txt data/sorted_qs.txt 16777216

run-ms: $(MS_OUT)
	@$(MS_OUT) data/input_1.txt data/sorted_ms.txt 16777216

run-all: run-qs run-ms

generate-256: $(GEN_OUT)
	@$(GEN_OUT) data/input_1.txt 256
	@$(GEN_OUT) data/input_2.txt 256
	@$(GEN_OUT) data/input_3.txt 256
	@echo "Generated 3 × 256MB input files"

# === Cleanup ====
clean:
	rm -rf $(BIN_DIR)
	@echo "🧹 Cleaned all binaries"

# === Build Each Separately ===
quick_sort: $(QS_OUT)
merge_sort: $(MS_OUT)
scripts: $(GEN_OUT) $(CMP_OUT)

.PHONY: all clean quick_sort merge_sort scripts run-qs run-ms run-all
