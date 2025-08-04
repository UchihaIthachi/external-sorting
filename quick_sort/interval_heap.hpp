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
    // Add internal storage (e.g., vector<pair<int, int>> or custom nodes)
    // and helper functions (siftUp, siftDown, etc.)
};
