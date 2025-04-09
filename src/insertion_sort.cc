#include "sorting.h"

void insertion_sort_interval(std::vector<std::pair<int,int>>& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        auto key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j].first > key.first) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void insertion_sort(std::vector<std::pair<int,int>>& arr) {
    insertion_sort_interval(arr,0,arr.size());
}