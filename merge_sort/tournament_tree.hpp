#pragma once
#include <vector>
#include <limits>

struct TreeNode {
    int key;
    int sourceId;
};

class TournamentTree {
public:
    TournamentTree(int k);
    void initialize(const std::vector<int>& initialKeys, const std::vector<int>& sourceIds);
    int getMinKey() const;
    int getMinSourceId() const;
    void replaceKey(int sourceId, int newKey);
    void removeMin();
    bool empty() const;
private:
    std::vector<TreeNode> tree;
    std::vector<int> leafMap; // Maps sourceId to leaf index
    int size;
    void siftUp(int index);
};
