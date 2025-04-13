#include <vector>
#include <utility>
#include <cstdlib>  // rand()
#include "sorting.h"

// 간단한 의사 랜덤 시드 없이 random pivot 선택
int random_index(int low, int high) {
    return low + std::abs(rand()) % (high - low + 1);
}

void partition_random(std::vector<std::pair<int,int>>& arr, int low, int high, int& i, int& j) {
    int pivot_index = random_index(low, high);
    std::swap(arr[low], arr[pivot_index]);
    auto pivot = arr[low];

    i = low - 1;
    j = high + 1;

    while (true) {
        do { ++i; } while (arr[i].first < pivot.first);
        do { --j; } while (arr[j].first > pivot.first);
        if (i >= j) break;
        std::swap(arr[i], arr[j]);
    }

    i = j + 1; // 재귀 파티션 구간 보정
}

void quick_sort__random(std::vector<std::pair<int,int>>& arr, int low, int high) {
    if (low < high) {
        int i, j;
        partition_random(arr, low, high, i, j);
        quick_sort__random(arr, low, j);   // 왼쪽 파티션
        quick_sort__random(arr, i, high);  // 오른쪽 파티션
    }
}

void quick_sort_random(std::vector<std::pair<int,int>>& arr) {
    if (!arr.empty())
        quick_sort__random(arr, 0, arr.size() - 1);
}
