#include "interval_heap.hpp"

IntervalHeap::IntervalHeap(size_t capacity)
    : capacity(capacity), currentSize(0) {
    heap.reserve(capacity / 2 + 1);  // Roughly half the capacity
}

void IntervalHeap::insert(int value) {
    if (isFull()) throw std::overflow_error("Heap is full");

    if (currentSize % 2 == 0) {
        heap.emplace_back(value, value);
    } else {
        auto& last = heap.back();
        if (value < last.first) last.first = value;
        else last.second = value;
    }
    currentSize++;
    siftUp(heap.size() - 1);
}

int IntervalHeap::getMin() {
    if (isEmpty()) throw std::underflow_error("Heap is empty");
    return heap[0].first;
}

int IntervalHeap::getMax() {
    if (isEmpty()) throw std::underflow_error("Heap is empty");
    return heap[0].second;
}

int IntervalHeap::removeMin() {
    if (isEmpty()) throw std::underflow_error("Heap is empty");
    int result = heap[0].first;

    if (heap[0].first == heap[0].second) {
        std::swap(heap[0], heap.back());
        heap.pop_back();
    } else {
        heap[0].first = heap.back().first;
        heap.pop_back();
    }
    currentSize--;
    siftDownMin(0);
    return result;
}

int IntervalHeap::removeMax() {
    if (isEmpty()) throw std::underflow_error("Heap is empty");
    int result = heap[0].second;

    if (heap[0].first == heap[0].second) {
        std::swap(heap[0], heap.back());
        heap.pop_back();
    } else {
        heap[0].second = heap.back().second;
        heap.pop_back();
    }
    currentSize--;
    siftDownMax(0);
    return result;
}

bool IntervalHeap::isEmpty() {
    return currentSize == 0;
}

bool IntervalHeap::isFull() {
    return currentSize >= capacity;
}

void IntervalHeap::siftUp(size_t index) {
    // Simplified: For full implementation, maintain heap shape
}

void IntervalHeap::siftDownMin(size_t index) {
    // Simplified: Rebalance min elements down
}

void IntervalHeap::siftDownMax(size_t index) {
    // Simplified: Rebalance max elements down
}
