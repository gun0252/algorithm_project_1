#include "sorting.h"
#include <vector>
#include <algorithm>

void comb_sort(std::vector<std::pair<int, int>>& arr) {
    int n = arr.size();
    double shrink = 1.3;
    int gap = n;
    bool swapped = true;
    while (gap > 1 || swapped) {
        gap = std::max(1, int(gap/shrink));
        swapped = false;
        for (int i = 0; i + gap < n; i++) {
            if (arr[i].first > arr[i + gap].first) {
                std::swap(arr[i], arr[i + gap]);
                swapped = true;
            }
        }
    }
}