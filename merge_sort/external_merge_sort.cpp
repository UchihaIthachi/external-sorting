#include "external_merge_sort.hpp"

void externalMergeSort(const std::string& inputFile, const std::string& outputFile, size_t memLimit) {
    const size_t BUF_SIZE = 1 << 20;  // 1 MB
    Buffer inputBuf(BUF_SIZE), outputBuf(BUF_SIZE);
    FileReader reader(inputFile, inputBuf);

    std::vector<std::string> runs;
    std::vector<int> treeKeys, pendingNextRun;
    std::vector<bool> keyFrozen;
    size_t maxKeys = (memLimit - 2 * BUF_SIZE) / sizeof(int);

    TournamentTree tree(maxKeys);
    while (treeKeys.size() < maxKeys && reader.hasNext()) {
        treeKeys.push_back(reader.next());
        keyFrozen.push_back(false);
    }

    std::vector<int> runIndex(treeKeys.size(), 0);
    tree.initialize(treeKeys, runIndex);

    int INF_KEY = INT_MAX;
    int lastOutput = INT_MIN;
    FileWriter runWriter;
    runWriter.openNew("run0.bin", outputBuf);
    int runCount = 0;

    while (!tree.empty()) {
        int minKey = tree.getMinKey();
        int src = tree.getMinSource();
        tree.removeKey();
        runWriter.write(minKey);
        if (runWriter.bufferFull()) runWriter.flush();
        lastOutput = minKey;

        if (reader.hasNext()) {
            int nextVal = reader.next();
            if (nextVal < lastOutput) {
                tree.replaceKey(INF_KEY, src);
                keyFrozen[src] = true;
                pendingNextRun.push_back(nextVal);
            } else {
                tree.replaceKey(nextVal, src);
            }
        }

        if (tree.getMinKey() == INF_KEY) {
            runWriter.flush(); runWriter.close();
            runs.push_back(runWriter.fileName());

            if (!pendingNextRun.empty()) {
                runCount++;
                std::string nextRun = "run" + std::to_string(runCount) + ".bin";
                runWriter.openNew(nextRun, outputBuf);

                treeKeys.assign(pendingNextRun.begin(), pendingNextRun.end());
                pendingNextRun.clear();
                keyFrozen.assign(treeKeys.size(), false);
                while (treeKeys.size() < maxKeys && reader.hasNext()) {
                    treeKeys.push_back(reader.next());
                    keyFrozen.push_back(false);
                }
                runIndex.assign(treeKeys.size(), 0);
                tree.initialize(treeKeys, runIndex);
                lastOutput = INT_MIN;
            }
        }
    }

    if (runWriter.isOpen()) {
        runWriter.flush(); runWriter.close();
        runs.push_back(runWriter.fileName());
    }
    reader.close();

    // === Multi-way Merging Phase ===
    int K = std::min(8, (int)(memLimit / BUF_SIZE / 2));
    std::vector<std::string> currentRuns = runs, nextRuns;
    int pass = 1;

    while (currentRuns.size() > 1) {
        nextRuns.clear();
        for (size_t i = 0; i < currentRuns.size(); i += K) {
            int groupSize = std::min(K, (int)(currentRuns.size() - i));
            std::vector<FileReader> runReaders;
            for (int j = 0; j < groupSize; ++j) {
                runReaders.emplace_back(currentRuns[i+j], Buffer(BUF_SIZE));
            }

            TournamentTree mergeTree(groupSize);
            std::vector<int> initKeys(groupSize), runIdx(groupSize);
            for (int j = 0; j < groupSize; ++j) {
                initKeys[j] = runReaders[j].hasNext() ? runReaders[j].next() : INF_KEY;
                runIdx[j] = j;
            }
            mergeTree.initialize(initKeys, runIdx);

            std::string mergedFile = "merge_pass" + std::to_string(pass) + "_run" + std::to_string(i/K) + ".bin";
            FileWriter mergedOut(mergedFile, outputBuf);

            while (!mergeTree.empty()) {
                int smallest = mergeTree.getMinKey();
                int srcRun = mergeTree.getMinSource();
                mergeTree.removeKey();
                mergedOut.write(smallest);
                if (mergedOut.bufferFull()) mergedOut.flush();

                if (runReaders[srcRun].hasNext()) {
                    mergeTree.replaceKey(runReaders[srcRun].next(), srcRun);
                } else {
                    mergeTree.replaceKey(INF_KEY, srcRun);
                }

                if (mergeTree.getMinKey() == INF_KEY) break;
            }

            mergedOut.flush(); mergedOut.close();
            for (auto& rr : runReaders) rr.close();
            nextRuns.push_back(mergedFile);
        }

        currentRuns.swap(nextRuns);
        pass++;
    }

    if (!currentRuns.empty()) {
        rename(currentRuns[0].c_str(), outputFile.c_str());
    }
}
