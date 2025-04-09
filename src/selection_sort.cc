#include "sorting.h"

// Selection Sort
void selection_sort(std::vector<std::pair<int,int>>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j].first < arr[min_idx].first) {
                min_idx = j;
            }
        }
        std::swap(arr[i], arr[min_idx]);
    }
}



