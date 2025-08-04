#ifndef INTERVAL_HEAP_HPP
#define INTERVAL_HEAP_HPP

#include <vector>
#include <cstddef>

class IntervalHeap {
public:
    IntervalHeap(size_t capacity);
    bool isFull() const;
    bool isEmpty() const;
    void insert(int value);
    int getMin() const;
    int getMax() const;
    int removeMin();
    int removeMax();

private:
    std::vector<int> heap;
    size_t capacity;
    void siftUp(size_t index);
    void siftDown(size_t index);
};

#endif
