#include "external_merge_sort.hpp"
#include "io_utils.hpp"
#include "tournament_tree.hpp"
#include "logger.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <memory>
#include <sstream>
#include <climits>

// External Merge Sort using Tournament Tree (min-winner tree)
void externalMergeSort(const std::string& inputFile, const std::string& outputFile, size_t memLimit) {
    std::cout << "=== External Merge Sort ===" << std::endl;
    std::cout << "Input file: " << inputFile << std::endl;
    std::cout << "Output file: " << outputFile << std::endl;
    std::cout << "Memory limit: " << memLimit << " bytes" << std::endl;

    // --------- Phase 1: Run Generation (Replacement Selection) ---------
    const size_t BUF_SIZE = 1 << 20; // 1 MB per buffer
    Buffer inputBuf(BUF_SIZE), outputBuf(BUF_SIZE);
    FileReader reader(inputFile, inputBuf);

    std::vector<std::string> runs;
    std::vector<int> treeKeys, pendingNextRun;
    
    size_t memForDataStructures = memLimit - (2 * BUF_SIZE);
    size_t memoryPerKey = sizeof(int) + (2 * sizeof(TreeNode)); // Simplified memory estimation
    size_t maxKeys = memForDataStructures / memoryPerKey;

    TournamentTree tree(static_cast<int>(maxKeys));
    std::cout << "Max keys in memory: " << maxKeys << std::endl;

    // Initial load: fill tournament tree with as many records as possible
    while (treeKeys.size() < maxKeys && reader.hasNext()) {
        treeKeys.push_back(reader.next());
    }
    std::cout << "Initial keys loaded: " << treeKeys.size() << std::endl;
    
    std::vector<int> sourceIds(treeKeys.size());
    for(size_t i = 0; i < treeKeys.size(); ++i) sourceIds[i] = i;
    tree.initialize(treeKeys, sourceIds);

    const int INF_KEY = std::numeric_limits<int>::max();
    int lastOutput = std::numeric_limits<int>::min();
    int runCount = 0;
    auto runWriter = std::make_unique<FileWriter>("run0.bin", outputBuf);

    std::cout << "--- Run Creation Phase ---" << std::endl;
    while (!tree.empty()) {
        int minKey = tree.getMinKey();
        int srcId = tree.getMinSourceId();
        tree.removeMin();
        runWriter->write(minKey);
        if (runWriter->bufferFull()) runWriter->flush();
        lastOutput = minKey;

        if (reader.hasNext()) {
            int nextVal = reader.next();
            if (nextVal < lastOutput) {
                tree.replaceKey(srcId, INF_KEY);
                pendingNextRun.push_back(nextVal);
            } else {
                tree.replaceKey(srcId, nextVal);
            }
        } else {
            // No more input, just keep removing min until tree is empty
            // The call to removeMin() at the start of the loop handles this
        }

        // When all remaining keys are INF_KEY, finish current run and start a new one
        if (tree.getMinKey() == INF_KEY) {
            runWriter->flush();
            runWriter->close();
            runs.push_back(runWriter->fileName());
            std::cout << "Finished run " << runCount << ": " << runs.back() << std::endl;

            if (!pendingNextRun.empty()) {
                runCount++;
                std::string nextRun = "run" + std::to_string(runCount) + ".bin";
                std::cout << "Starting new run " << runCount << ": " << nextRun << std::endl;
                runWriter = std::make_unique<FileWriter>(nextRun, outputBuf);

                treeKeys.assign(pendingNextRun.begin(), pendingNextRun.end());
                pendingNextRun.clear();
                
                while (treeKeys.size() < maxKeys && reader.hasNext()) {
                    treeKeys.push_back(reader.next());
                }

                sourceIds.resize(treeKeys.size());
                for(size_t i = 0; i < treeKeys.size(); ++i) sourceIds[i] = i;
                tree.initialize(treeKeys, sourceIds);
                lastOutput = std::numeric_limits<int>::min();
            }
        }
    }

    if (runWriter && runWriter->isOpen()) {
        runWriter->flush();
        runWriter->close();
        runs.push_back(runWriter->fileName());
    }
    reader.close();

    // --------- Phase 2: Multi-way Merge (K-way Merge with Tournament Tree) ---------
    std::cout << "--- Multi-way Merging Phase ---" << std::endl;
    int K = std::min(8, static_cast<int>(memLimit / BUF_SIZE / 2));
    std::vector<std::string> currentRuns = runs, nextRuns;
    int pass = 1;

    while (currentRuns.size() > 1) {
        std::cout << "Merge pass " << pass << ": " << currentRuns.size()
                  << " runs, merging " << K << " at a time." << std::endl;
        nextRuns.clear();

        for (size_t i = 0; i < currentRuns.size(); i += K) {
            int groupSize = std::min(K, static_cast<int>(currentRuns.size() - i));
            std::cout << "Merging group of " << groupSize << " runs." << std::endl;

            std::vector<std::unique_ptr<Buffer>> buffers;
            std::vector<std::unique_ptr<FileReader>> runReaders;
            for (int j = 0; j < groupSize; ++j) {
                buffers.push_back(std::make_unique<Buffer>(BUF_SIZE));
                runReaders.push_back(std::make_unique<FileReader>(currentRuns[i + j], *buffers.back()));
            }

            TournamentTree mergeTree(groupSize);
            std::vector<int> initKeys(groupSize), sourceIds(groupSize);
            for (int j = 0; j < groupSize; ++j) {
                initKeys[j] = runReaders[j]->hasNext() ? runReaders[j]->next() : INF_KEY;
                sourceIds[j] = j;
            }
            mergeTree.initialize(initKeys, sourceIds);

            std::string mergedFile = "merge_pass" + std::to_string(pass)
                                   + "_run" + std::to_string(i / K) + ".bin";
            FileWriter mergedOut(mergedFile, outputBuf);

            while (!mergeTree.empty()) {
                int smallest = mergeTree.getMinKey();
                int srcRun = mergeTree.getMinSourceId();
                
                if (smallest == INF_KEY) break;

                mergedOut.write(smallest);
                if (mergedOut.bufferFull()) mergedOut.flush();

                if (runReaders[srcRun]->hasNext()) {
                    mergeTree.replaceKey(srcRun, runReaders[srcRun]->next());
                } else {
                    mergeTree.replaceKey(srcRun, INF_KEY);
                }
            }

            mergedOut.flush();
            mergedOut.close();
            for (auto& rr : runReaders) rr->close();
            nextRuns.push_back(mergedFile);

            // Optional: Delete old temporary runs to save disk space
            for (int j = 0; j < groupSize; ++j) {
                if (remove(currentRuns[i + j].c_str()) != 0) {
                    std::stringstream ss;
                    ss << "Error deleting file: " << currentRuns[i + j];
                    LOG_DEBUG(ss.str());
                }
            }
        }
        currentRuns.swap(nextRuns);
        pass++;
    }

    if (!currentRuns.empty()) {
        rename(currentRuns[0].c_str(), outputFile.c_str());
    }
    std::cout << "Merge sort completed." << std::endl;
}
