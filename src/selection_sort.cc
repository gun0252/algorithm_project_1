#include "sorting.h"

// Straight Selection Sort (select max to end)
void selection_sort(std::vector<std::pair<int,int>>& arr) {
    int n = arr.size();
    for (int j = n - 1; j > 0; --j) {
        int max_idx = j;
        for (int i = j - 1; i >= 0; --i) {
            if (arr[i].first > arr[max_idx].first) {
                max_idx = i;
            }
        }
        std::swap(arr[j], arr[max_idx]);
    }
}
