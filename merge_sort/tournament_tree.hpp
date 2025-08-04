#pragma once
#include <vector>
#include <limits>

struct TreeNode {
    int key;
    int sourceRun;
};

class TournamentTree {
public:
    TournamentTree(int k);
    void initialize(const std::vector<int>& initialKeys, const std::vector<int>& runIdx);
    int getMinKey() const;
    int getMinSource() const;
    void replaceKey(int newKey, int runIndex);
    void removeKey();
    bool empty() const;

private:
    std::vector<TreeNode> tree;
    int size;
    void buildTree();
    void siftUp(int index);
};
