#include "sorting.h"

// Max-heapify
void max_heapify(std::vector<std::pair<int,int>>& arr, int start, int heap_size, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap_size && arr[start + left].first > arr[start + largest].first)
        largest = left;
    if (right < heap_size && arr[start + right].first > arr[start + largest].first)
        largest = right;

    if (largest != i) {
        std::swap(arr[start + i], arr[start + largest]);
        max_heapify(arr, start, heap_size, largest);
    }
}

void build_max_heap(std::vector<std::pair<int,int>>& arr, int start, int end) {
    int heap_size = end - start + 1;
    for (int i = heap_size / 2 - 1; i >= 0; --i) {
        max_heapify(arr, start, heap_size, i);
    }
}

void heap_sort_interval(std::vector<std::pair<int,int>>& arr, int left, int right) {
    int heap_size = right - left + 1;
    build_max_heap(arr, left, right);

    for (int i = heap_size - 1; i >= 1; --i) {
        std::swap(arr[left], arr[left + i]);
        max_heapify(arr, left, i, 0);  // heap_size = i
    }
}

void heap_sort(std::vector<std::pair<int,int>>& arr) {
    heap_sort_interval(arr, 0, arr.size() - 1);
}

