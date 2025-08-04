#ifndef INTERVAL_HEAP_HPP
#define INTERVAL_HEAP_HPP

#include <vector>
#include <stdexcept>
#include <utility>
#include <algorithm>

class IntervalHeap {
public:
    IntervalHeap(size_t capacity);
    void insert(int value);
    int getMin();
    int getMax();
    int removeMin();
    int removeMax();
    bool isEmpty();
    bool isFull();

private:
    std::vector<std::pair<int, int>> heap;  // Each node stores [min, max]
    size_t currentSize;
    size_t capacity;

    void siftUp(size_t index);
    void siftDownMin(size_t index);
    void siftDownMax(size_t index);
};

#endif // INTERVAL_HEAP_HPP
