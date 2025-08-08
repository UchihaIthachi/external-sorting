#include "tournament_tree.hpp"
TournamentTree::TournamentTree(int k) : size(k), tree(2 * k - 1) {}

void TournamentTree::initialize(const std::vector<int>& initialKeys, const std::vector<int>& runIdx) {
    for (int i = 0; i < size; ++i)
        tree[size - 1 + i] = {initialKeys[i], runIdx[i]};
    for (int i = size - 2; i >= 0; --i) {
        const TreeNode& left = tree[2 * i + 1];
        const TreeNode& right = tree[2 * i + 2];
        tree[i] = (left.key <= right.key) ? left : right;
    }
}

int TournamentTree::getMinKey() const { return tree[0].key; }
int TournamentTree::getMinSource() const { return tree[0].sourceRun; }

void TournamentTree::replaceKey(int newKey, int runIndex) {
    int idx = size - 1;
    for (; idx < 2 * size - 1; ++idx) {
        if (tree[idx].sourceRun == runIndex) break;
    }
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

void TournamentTree::removeKey() {
    replaceKey(std::numeric_limits<int>::max(), getMinSource());
}

bool TournamentTree::empty() const {
    return tree[0].key == std::numeric_limits<int>::max();
}
