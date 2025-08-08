#include "interval_heap.hpp"
#include "logger.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <sstream>

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
    std::stringstream ss;
    ss << "  insert(" << value << ")";
    LOG_DEBUG(ss.str());

    if (isFull()) {
        std::stringstream ss_full;
        ss_full << "Heap is full, cannot insert " << value;
        LOG_DEBUG(ss_full.str());
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
    LOG_DEBUG("  removeMin called");
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
        LOG_DEBUG("  fixup in removeMin");
        std::swap(heap[0].left, heap[0].right);
    }

    return minVal;
}

int IntervalHeap::removeMax() {
    LOG_DEBUG("  removeMax called");
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
        LOG_DEBUG("  fixup in removeMax");
        std::swap(heap[0].left, heap[0].right);
    }

    return maxVal;
}

void IntervalHeap::siftUpMin(size_t i) {
    std::stringstream ss;
    ss << "  siftUpMin(" << i << ")";
    LOG_DEBUG(ss.str());
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
    std::stringstream ss;
    ss << "  siftUpMax(" << i << ")";
    LOG_DEBUG(ss.str());
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
    std::stringstream ss;
    ss << "  siftDownMin(" << i << ")";
    LOG_DEBUG(ss.str());
    size_t n = heap.size();
    while (true) {
        size_t left = leftChild(i);
        size_t right = rightChild(i);
        size_t smallest = i;

        if (left < n && heap[left].left < heap[smallest].left) smallest = left;
        if (right < n && heap[right].left < heap[smallest].left) smallest = right;

        if (smallest != i) {
            std::stringstream ss_swap;
            ss_swap << "    siftDownMin swap " << i << " (" << heap[i].left << ") with " << smallest << " (" << heap[smallest].left << ")";
            LOG_DEBUG(ss_swap.str());
            std::swap(heap[i].left, heap[smallest].left);
            if (!heap[i].hasSingle && heap[i].left > heap[i].right) {
                std::stringstream ss_fix;
                ss_fix << "    fix interval at " << i;
                LOG_DEBUG(ss_fix.str());
                std::swap(heap[i].left, heap[i].right);
                siftUpMax(i);
            }
            if (!heap[smallest].hasSingle && heap[smallest].left > heap[smallest].right) {
                std::stringstream ss_fix;
                ss_fix << "    fix interval at " << smallest;
                LOG_DEBUG(ss_fix.str());
                std::swap(heap[smallest].left, heap[smallest].right);
                siftUpMax(smallest);
            }
            i = smallest;
        } else {
            break;
        }
    }
}

void IntervalHeap::siftDownMax(size_t i) {
    std::stringstream ss;
    ss << "  siftDownMax(" << i << ")";
    LOG_DEBUG(ss.str());
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
            std::stringstream ss_swap;
            ss_swap << "    siftDownMax swap " << i << " with " << largest;
            LOG_DEBUG(ss_swap.str());
            int& val_i = heap[i].hasSingle ? heap[i].left : heap[i].right;
            int& val_largest = heap[largest].hasSingle ? heap[largest].left : heap[largest].right;
            std::swap(val_i, val_largest);

            if (!heap[i].hasSingle && heap[i].left > heap[i].right) {
                std::swap(heap[i].left, heap[i].right);
                siftUpMin(i);
            }
            if (!heap[largest].hasSingle && heap[largest].left > heap[largest].right) {
                std::swap(heap[largest].left, heap[largest].right);
                siftUpMin(largest);
            }
            i = largest;
        } else {
            break;
        }
    }
}
