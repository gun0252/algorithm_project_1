#include "sorting.h"

int hoare_partition(std::vector<std::pair<int,int>>& arr, int low, int high) {
    auto pivot = arr[low];
    int i = low - 1;
    int j = high + 1;

    while (true) {
        do {
            ++i;
        } while (arr[i].first < pivot.first);

        do {
            --j;
        } while (arr[j].first > pivot.first);

        if (i >= j) return j;

        swap(arr[i], arr[j]);
    }
}


// Quick Sort main
void quick_sort_recursive(std::vector<std::pair<int,int>>& arr, int low, int high) {
    if (low < high) {
        int p = hoare_partition(arr, low, high);
        quick_sort_recursive(arr, low, p);
        quick_sort_recursive(arr, p + 1, high);
    }
}

void quick_sort(std::vector<std::pair<int,int>>& arr) {
    quick_sort_recursive(arr, 0, arr.size() - 1);
}
