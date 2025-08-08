#include "tournament_tree.hpp"
#include <iostream>

TournamentTree::TournamentTree(int k) : size(k) {
    if (k > 0) {
        tree.resize(2 * k - 1);
        leafMap.resize(k);
    }
}

void TournamentTree::initialize(const std::vector<int>& initialKeys, const std::vector<int>& sourceIds) {
    // size is k, the capacity. initialKeys.size() is the number of actual keys.
    for (size_t i = 0; i < initialKeys.size(); ++i) {
        int leafIdx = size - 1 + i;
        tree[leafIdx] = {initialKeys[i], sourceIds[i]};
        if (sourceIds[i] >= 0) {
            if (sourceIds[i] >= leafMap.size()) {
                leafMap.resize(sourceIds[i] + 1);
            }
            leafMap[sourceIds[i]] = leafIdx;
        }
    }
    // Fill the rest of the leaves with INF
    for (size_t i = initialKeys.size(); i < size; ++i) {
        int leafIdx = size - 1 + i;
        tree[leafIdx] = {std::numeric_limits<int>::max(), -1}; // -1 for sourceId
    }

    // Build the rest of the tree
    for (int i = size - 2; i >= 0; --i) {
        const TreeNode& left = tree[2 * i + 1];
        const TreeNode& right = tree[2 * i + 2];
        tree[i] = (left.key <= right.key) ? left : right;
    }
}

int TournamentTree::getMinKey() const { return tree[0].key; }
int TournamentTree::getMinSourceId() const { return tree[0].sourceId; }

void TournamentTree::replaceKey(int sourceId, int newKey) {
    if (sourceId < 0 || static_cast<size_t>(sourceId) >= leafMap.size()) {
        // Handle error: sourceId out of bounds
        std::cerr << "Error: sourceId " << sourceId << " is out of bounds." << std::endl;
        return;
    }
    int idx = leafMap[sourceId];
    tree[idx].key = newKey;
    siftUp(idx);
}

void TournamentTree::siftUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        TreeNode& left = tree[2 * parent + 1];
        TreeNode& right = tree[2 * parent + 2];
        tree[parent] = (left.key <= right.key) ? left : right;
        index = parent;
    }
}

void TournamentTree::removeMin() {
    int sourceId = getMinSourceId();
    if (sourceId != -1) {
        replaceKey(sourceId, std::numeric_limits<int>::max());
    }
}

bool TournamentTree::empty() const {
    return size == 0 || tree[0].key == std::numeric_limits<int>::max();
}
