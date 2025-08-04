#include "interval_heap.hpp"
#include <iostream>
#include <algorithm>

IntervalHeap::IntervalHeap(size_t capacity) : capacity(capacity) {
    heap.reserve(capacity);
}

bool IntervalHeap::isFull() const {
    return heap.size() == capacity;
}

bool IntervalHeap::isEmpty() const {
    return heap.empty();
}

void IntervalHeap::insert(int value) {
    if (isFull()) {
        std::cerr << "Heap is full. Cannot insert " << value << std::endl;
        return;
    }
    heap.push_back(value);
    siftUp(heap.size() - 1);
}

int IntervalHeap::getMin() const {
    if (isEmpty()) {
        std::cerr << "Heap is empty. Cannot get min." << std::endl;
        return -1; // Or throw an exception
    }
    return heap[0];
}

int IntervalHeap::getMax() const {
    if (isEmpty()) {
        std::cerr << "Heap is empty. Cannot get max." << std::endl;
        return -1; // Or throw an exception
    }
    if (heap.size() == 1) {
        return heap[0];
    }
    return heap[1];
}

int IntervalHeap::removeMin() {
    if (isEmpty()) {
        std::cerr << "Heap is empty. Cannot remove min." << std::endl;
        return -1; // Or throw an exception
    }
    int minVal = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) {
        siftDown(0);
    }
    return minVal;
}

int IntervalHeap::removeMax() {
    if (isEmpty()) {
        std::cerr << "Heap is empty. Cannot remove max." << std::endl;
        return -1; // Or throw an exception
    }
    int maxVal;
    if (heap.size() == 1) {
        maxVal = heap[0];
        heap.pop_back();
    } else {
        maxVal = heap[1];
        heap[1] = heap.back();
        heap.pop_back();
        if (heap.size() > 1) {
            siftDown(1);
        }
    }
    return maxVal;
}

void IntervalHeap::siftUp(size_t index) {
    if (index == 0) return;
    size_t parentIndex = (index - 1) / 2;
    if (index % 2 == 1) { // On the min-heap side
        if (heap[index] > heap[parentIndex * 2 + 2]) {
            std::swap(heap[index], heap[parentIndex * 2 + 2]);
            siftUp(parentIndex * 2 + 2);
        }
        if (heap[index] < heap[parentIndex * 2 + 1]) {
            std::swap(heap[index], heap[parentIndex * 2 + 1]);
            siftUp(parentIndex * 2 + 1);
        }
    } else { // On the max-heap side
        if (heap[index] < heap[parentIndex * 2 + 1]) {
            std::swap(heap[index], heap[parentIndex * 2 + 1]);
            siftUp(parentIndex * 2 + 1);
        }
        if (heap[index] > heap[parentIndex * 2 + 2]) {
            std::swap(heap[index], heap[parentIndex * 2 + 2]);
            siftUp(parentIndex * 2 + 2);
        }
    }
}

void IntervalHeap::siftDown(size_t index) {
    size_t leftChild = 2 * index + 1;
    size_t rightChild = 2 * index + 2;
    size_t smallest = index;

    if (leftChild < heap.size() && heap[leftChild] < heap[smallest]) {
        smallest = leftChild;
    }
    if (rightChild < heap.size() && heap[rightChild] < heap[smallest]) {
        smallest = rightChild;
    }

    if (smallest != index) {
        std::swap(heap[index], heap[smallest]);
        siftDown(smallest);
    }
}
