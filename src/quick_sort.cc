#include "sorting.h"

// Partition function returning i and j by reference (fixed)
void partition(std::vector<std::pair<int,int>>& arr, int low, int high, int& i, int& j) {
    auto pivot = arr[low];
    i = low - 1;
    j = high + 1;

    while (true) {
        do {
            ++i;
        } while (arr[i].first < pivot.first);

        do {
            --j;
        } while (arr[j].first > pivot.first);

        if (i >= j) break;

        std::swap(arr[i], arr[j]);
    }
    i = j + 1;
}


void quick_sort_recursive(std::vector<std::pair<int,int>>& arr, int low, int high) {
    if (low < high) {
        int i, j;
        partition(arr, low, high, i, j);
        quick_sort_recursive(arr, low, j);   // Left part
        quick_sort_recursive(arr, i, high);  // Right part
    }
}

void quick_sort(std::vector<std::pair<int,int>>& arr) {
    if (!arr.empty())
        quick_sort_recursive(arr, 0, arr.size() - 1);
}
