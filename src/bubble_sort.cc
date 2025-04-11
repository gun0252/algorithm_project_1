#include "sorting.h"
#include <vector>
#include <algorithm>

// Bubble Sort
void bubble_sort(std::vector<std::pair<int,int>>& arr) {
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j].first > arr[j + 1].first) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}