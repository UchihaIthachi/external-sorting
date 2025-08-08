#include "interval_heap.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>

IntervalHeap::IntervalHeap(size_t capacity) : capacity(capacity) {
    heap.reserve(capacity / 2 + 1);
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
    std::cerr << "  insert(" << value << ")" << std::endl;
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
        if (value < lastNode.left) {
            lastNode.right = lastNode.left;
            lastNode.left = value;
        } else {
            lastNode.right = value;
        }
        lastNode.hasSingle = false;

        siftUpMin(heap.size() - 1);
        siftUpMax(heap.size() - 1);
    } else {
        heap.emplace_back(value);
        siftUpMin(heap.size() - 1);
        siftUpMax(heap.size() - 1);
    }
}

int IntervalHeap::removeMin() {
    std::cerr << "  removeMin called" << std::endl;
    if (isEmpty()) throw std::runtime_error("Heap is empty, removeMin()");
    int minVal = heap[0].left;

    if (heap.size() == 1 && heap[0].hasSingle) {
        heap.pop_back();
        return minVal;
    }

    Node& lastNode = heap.back();

    if (lastNode.hasSingle) {
        heap[0].left = lastNode.left;
        heap.pop_back();
    } else {
        heap[0].left = lastNode.left;
        lastNode.left = lastNode.right;
        lastNode.hasSingle = true;
    }

    siftDownMin(0);

    if (!heap.empty() && !heap[0].hasSingle && heap[0].left > heap[0].right) {
        std::cerr << "  fixup in removeMin" << std::endl;
        std::swap(heap[0].left, heap[0].right);
    }

    return minVal;
}

int IntervalHeap::removeMax() {
    std::cerr << "  removeMax called" << std::endl;
    if (isEmpty()) throw std::runtime_error("Heap is empty, removeMax()");
    int maxVal;

    if (heap.size() == 1 && heap[0].hasSingle) {
        maxVal = heap[0].left;
        heap.pop_back();
        return maxVal;
    }

    maxVal = getMax();

    Node& lastNode = heap.back();

    if (lastNode.hasSingle) {
        heap[0].right = lastNode.left;
        heap.pop_back();
    } else {
        heap[0].right = lastNode.right;
        lastNode.right = 0;
        lastNode.hasSingle = true;
    }

    siftDownMax(0);

    if (!heap.empty() && !heap[0].hasSingle && heap[0].left > heap[0].right) {
        std::cerr << "  fixup in removeMax" << std::endl;
        std::swap(heap[0].left, heap[0].right);
    }

    return maxVal;
}

void IntervalHeap::siftUpMin(size_t i) {
    std::cerr << "  siftUpMin(" << i << ")" << std::endl;
    while (i > 0) {
        size_t p = parent(i);
        if (heap[i].left < heap[p].left) {
            std::swap(heap[i].left, heap[p].left);
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
    std::cerr << "  siftUpMax(" << i << ")" << std::endl;
    while (i > 0) {
        size_t p = parent(i);
        int currMax = heap[i].hasSingle ? heap[i].left : heap[i].right;
        int parentMax = heap[p].hasSingle ? heap[p].left : heap[p].right;

        if (currMax > parentMax) {
            int& val_i = heap[i].hasSingle ? heap[i].left : heap[i].right;
            int& val_p = heap[p].hasSingle ? heap[p].left : heap[p].right;
            std::swap(val_i, val_p);

            if (!heap[i].hasSingle && heap[i].left > heap[i].right) {
                std::swap(heap[i].left, heap[i].right);
            }
            if (!heap[p].hasSingle && heap[p].left > heap[p].right) {
                std::swap(heap[p].left, heap[p].right);
            }
            i = p;
        } else {
            break;
        }
    }
}

void IntervalHeap::siftDownMin(size_t i) {
    std::cerr << "  siftDownMin(" << i << ")" << std::endl;
    size_t n = heap.size();
    while (true) {
        size_t left = leftChild(i);
        size_t right = rightChild(i);
        size_t smallest = i;

        if (left < n && heap[left].left < heap[smallest].left) smallest = left;
        if (right < n && heap[right].left < heap[smallest].left) smallest = right;

        if (smallest != i) {
            std::cerr << "    siftDownMin swap " << i << " (" << heap[i].left << ") with " << smallest << " (" << heap[smallest].left << ")" << std::endl;
            std::swap(heap[i].left, heap[smallest].left);
            if (!heap[i].hasSingle && heap[i].left > heap[i].right) {
                std::cerr << "    fix interval at " << i << std::endl;
                std::swap(heap[i].left, heap[i].right);
            }
            if (!heap[smallest].hasSingle && heap[smallest].left > heap[smallest].right) {
                std::cerr << "    fix interval at " << smallest << std::endl;
                std::swap(heap[smallest].left, heap[smallest].right);
            }
            i = smallest;
        } else {
            break;
        }
    }
}

void IntervalHeap::siftDownMax(size_t i) {
    std::cerr << "  siftDownMax(" << i << ")" << std::endl;
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
            std::cerr << "    siftDownMax swap " << i << " with " << largest << std::endl;
            int& val_i = heap[i].hasSingle ? heap[i].left : heap[i].right;
            int& val_largest = heap[largest].hasSingle ? heap[largest].left : heap[largest].right;
            std::swap(val_i, val_largest);

            if (!heap[i].hasSingle && heap[i].left > heap[i].right) {
                std::swap(heap[i].left, heap[i].right);
            }
            if (!heap[largest].hasSingle && heap[largest].left > heap[largest].right) {
                std::swap(heap[largest].left, heap[largest].right);
            }
            i = largest;
        } else {
            break;
        }
    }
}
