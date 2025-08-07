#ifndef INTERVAL_HEAP_HPP
#define INTERVAL_HEAP_HPP

#include <vector>
#include <cstddef>
#include <stdexcept>

class IntervalHeap {
public:
    explicit IntervalHeap(size_t capacity);
    bool isFull() const;
    bool isEmpty() const;
    void insert(int value);
    int getMin() const;
    int getMax() const;
    int removeMin();
    int removeMax();

private:
    struct Node {
        int left;  // min element of interval
        int right; // max element of interval
        bool hasSingle; // true if node holds only one element (left)
        Node(int val) : left(val), right(val), hasSingle(true) {}
        Node(int l, int r) : left(l), right(r), hasSingle(false) {}
    };

    std::vector<Node> heap;
    size_t capacity;

    void siftUpMin(size_t index);
    void siftUpMax(size_t index);
    void siftDownMin(size_t index);
    void siftDownMax(size_t index);

    void swapNodes(size_t i, size_t j);

    size_t parent(size_t i) const { return (i - 1) / 2; }
    size_t leftChild(size_t i) const { return 2 * i + 1; }
    size_t rightChild(size_t i) const { return 2 * i + 2; }
};

#endif

