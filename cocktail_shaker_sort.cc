#include "sorting.h"
#include <vector>
#include <algorithm>

// Cocktail_shaker_sort
void cocktail_shaker_sort(std::vector<std::pair<int,int>>& arr) {
    int i = 0;
    int j = arr.size() - 1;

    while (i < j) {
        bool swapped = false;

        for (int odd = i; odd < j; odd++) {
            if (arr[odd].first > arr[odd + 1].first) {
                std::swap(arr[odd], arr[odd + 1]);
                swapped = true;
            }
        }
        j--;

        for (int even = j; even > i; even--) {
            if (arr[even - 1].first > arr[even].first) {
                std::swap(arr[even - 1], arr[even]);
                swapped = true;
            }
        }
        i++; 

        if (!swapped) break;
    }
}
