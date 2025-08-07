#include "interval_heap.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>

IntervalHeap::IntervalHeap(size_t capacity) : capacity(capacity) {
    heap.reserve(capacity / 2 + 1);  // approx capacity / 2 nodes
}

bool IntervalHeap::isFull() const {
    return heap.size() * 2 >= capacity;
}

bool IntervalHeap::isEmpty() const {
    return heap.empty();
}

int IntervalHeap::getMin() const {
    if (isEmpty()) throw std::runtime_error("Heap is empty, getMin()");
    return heap[0].left;
}

int IntervalHeap::getMax() const {
    if (isEmpty()) throw std::runtime_error("Heap is empty, getMax()");
    if (heap[0].hasSingle) return heap[0].left;
    return heap[0].right;
}

void IntervalHeap::insert(int value) {
    if (isFull()) {
        std::cerr << "Heap is full, cannot insert " << value << std::endl;
        return;
    }
    if (heap.empty()) {
        heap.emplace_back(value);
        return;
    }

    Node& lastNode = heap.back();

    if (lastNode.hasSingle) {
        // Insert second element into last node and order min/max
        if (value < lastNode.left) {
            lastNode.right = lastNode.left;
            lastNode.left = value;
        } else {
            lastNode.right = value;
        }
        lastNode.hasSingle = false;

        // Sift up min side and max side
        siftUpMin(heap.size() - 1);
        siftUpMax(heap.size() - 1);
    } else {
        // Create new node with single element
        heap.emplace_back(value);
        // No need to sift, it's a single element node
        // But must maintain heap property upwards
        siftUpMin(heap.size() - 1);
        siftUpMax(heap.size() - 1);
    }
}

int IntervalHeap::removeMin() {
    if (isEmpty()) throw std::runtime_error("Heap is empty, removeMin()");

    int minVal = heap[0].left;

    if (heap.size() == 1 && heap[0].hasSingle) {
        heap.pop_back();
        return minVal;
    }

    Node& lastNode = heap.back();

    if (lastNode.hasSingle) {
        // Move lastNode.left to root.left
        heap[0].left = lastNode.left;
        heap.pop_back();
    } else {
        // Move lastNode.left to root.left, then
        // lastNode.right becomes new single element
        heap[0].left = lastNode.left;
        lastNode.left = lastNode.right;
        lastNode.hasSingle = true;
    }

    siftDownMin(0);

    // Fix min/max order in root node if violated
    if (!heap.empty() && !heap[0].hasSingle && heap[0].left > heap[0].right) {
        std::swap(heap[0].left, heap[0].right);
    }

    return minVal;
}

int IntervalHeap::removeMax() {
    if (isEmpty()) throw std::runtime_error("Heap is empty, removeMax()");

    int maxVal;

    if (heap.size() == 1 && heap[0].hasSingle) {
        maxVal = heap[0].left;
        heap.pop_back();
        return maxVal;
    }

    maxVal = heap[0].right;

    Node& lastNode = heap.back();

    if (lastNode.hasSingle) {
        heap[0].right = lastNode.left;
        heap.pop_back();
    } else {
        heap[0].right = lastNode.right;
        lastNode.right = lastNode.left;
        lastNode.hasSingle = true;
    }

    siftDownMax(0);

    if (!heap.empty() && !heap[0].hasSingle && heap[0].left > heap[0].right) {
        std::swap(heap[0].left, heap[0].right);
    }

    return maxVal;
}

void IntervalHeap::siftUpMin(size_t i) {
    while (i > 0) {
        size_t p = parent(i);
        if (heap[i].left < heap[p].left) {
            std::swap(heap[i].left, heap[p].left);
            // Make sure max elements stay valid
            if (!heap[i].hasSingle && heap[i].left > heap[i].right)
                std::swap(heap[i].left, heap[i].right);
            if (!heap[p].hasSingle && heap[p].left > heap[p].right)
                std::swap(heap[p].left, heap[p].right);
            i = p;
        } else {
            break;
        }
    }
}

void IntervalHeap::siftUpMax(size_t i) {
    while (i > 0) {
        size_t p = parent(i);
        // For max side we compare right elements if exist, else left
        int currMax = heap[i].hasSingle ? heap[i].left : heap[i].right;
        int parentMax = heap[p].hasSingle ? heap[p].left : heap[p].right;

        if (currMax > parentMax) {
            if (heap[i].hasSingle) {
                if (heap[p].hasSingle) {
                    std::swap(heap[i].left, heap[p].left);
                } else {
                    std::swap(heap[i].left, heap[p].right);
                }
            } else {
                if (heap[p].hasSingle) {
                    std::swap(heap[i].right, heap[p].left);
                } else {
                    std::swap(heap[i].right, heap[p].right);
                }
            }
            i = p;
        } else {
            break;
        }
    }
}

void IntervalHeap::siftDownMin(size_t i) {
    size_t n = heap.size();
    while (true) {
        size_t left = leftChild(i);
        size_t right = rightChild(i);
        size_t smallest = i;

        if (left < n && heap[left].left < heap[smallest].left) smallest = left;
        if (right < n && heap[right].left < heap[smallest].left) smallest = right;

        if (smallest != i) {
            std::swap(heap[i].left, heap[smallest].left);
            // Fix max if min and max out of order
            if (!heap[i].hasSingle && heap[i].left > heap[i].right)
                std::swap(heap[i].left, heap[i].right);
            if (!heap[smallest].hasSingle && heap[smallest].left > heap[smallest].right)
                std::swap(heap[smallest].left, heap[smallest].right);
            i = smallest;
        } else {
            break;
        }
    }
}

void IntervalHeap::siftDownMax(size_t i) {
    size_t n = heap.size();
    while (true) {
        size_t left = leftChild(i);
        size_t right = rightChild(i);
        size_t largest = i;

        auto getRightMax = [&](size_t idx) -> int {
            return heap[idx].hasSingle ? heap[idx].left : heap[idx].right;
        };

        if (left < n && getRightMax(left) > getRightMax(largest)) largest = left;
        if (right < n && getRightMax(right) > getRightMax(largest)) largest = right;

        if (largest != i) {
            if (heap[i].hasSingle) {
                if (heap[largest].hasSingle) {
                    std::swap(heap[i].left, heap[largest].left);
                } else {
                    std::swap(heap[i].left, heap[largest].right);
                }
            } else {
                if (heap[largest].hasSingle) {
                    std::swap(heap[i].right, heap[largest].left);
                } else {
                    std::swap(heap[i].right, heap[largest].right);
                }
            }
            i = largest;
        } else {
            break;
        }
    }
}
