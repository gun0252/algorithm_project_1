#include "sorting.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>
#include <iostream>
const int INSERTION_SORT_THRESHOLD = 16;
int get_depth_limit(int n) {
    return 2 * static_cast<int>(std::log2(n));
}

int median_of_three_index(std::vector<std::pair<int,int>>& arr, int a, int b, int c) {
    if (arr[a].first < arr[b].first) {
        if (arr[b].first < arr[c].first) return b;
        else if (arr[a].first < arr[c].first) return c;
        else return a;
    } else {
        if (arr[a].first < arr[c].first) return a;
        else if (arr[b].first < arr[c].first) return c;
        else return b;
    }
}


void insertion_sort_interval(std::vector<std::pair<int,int>>& arr, int left, int right);
void heap_sort_interval(std::vector<std::pair<int,int>>& arr, int left, int right);

int partition(std::vector<std::pair<int, int>>& arr, int left, int right, int pivot_val) {
    int i = left;
    int j = right - 1;

    while (true) {
        // 왼쪽에서 pivot보다 크거나 같은 값 찾기
        while (i <= j && arr[i].first < pivot_val) i++;

        // 오른쪽에서 pivot보다 작은 값 찾기
        while (i <= j && arr[j].first >= pivot_val) j--;

        if (i >= j) break;

        std::swap(arr[i], arr[j]);
        i++;
        j--;
    }

    return i; // pivot 기준으로 나뉜 경계 반환
}

void intro_sort_recursive(std::vector<std::pair<int,int>>& arr, int left, int right, int depth_limit) {
    while (right - left > INSERTION_SORT_THRESHOLD) {
        if (depth_limit == 0) {
            heap_sort_interval(arr, left, right);
            return;
        }
        depth_limit--;

        int mid = left + (right - left) / 2;
        int pivot_idx = median_of_three_index(arr, left, mid, right - 1);
        int pivot_val = arr[pivot_idx].first;

        int cut = partition(arr, left, right, pivot_val);
        intro_sort_recursive(arr, cut, right, depth_limit);
        right = cut;
    }
}


void intro_sort(std::vector<std::pair<int,int>>& arr) {
    int n = arr.size();
    int depth_limit = get_depth_limit(n);
    intro_sort_recursive(arr, 0, n , depth_limit);
    insertion_sort_interval(arr, 0, n-1);
}
