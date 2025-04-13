#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>
#include <limits>

static const int MIN_RUN = 32;
static const int GALLOP_THRESHOLD = 7;

static std::vector<std::pair<int,int>> temp;

void binary_insertion(std::vector<std::pair<int,int>>& arr, int start, int end) {
    std::pair<int,int> target = arr[end];
    int low = start, high = end - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] <= target) low = mid + 1;
        else high = mid - 1;
    }
    for (int j = end; j > low; --j) arr[j] = arr[j - 1];
    arr[low] = target;
}

void binary_insertion_reverse(std::vector<std::pair<int,int>>& arr, int start, int end) {
    std::pair<int,int> target = arr[end];
    int low = start, high = end - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] >= target) low = mid + 1;
        else high = mid - 1;
    }
    for (int j = end; j > low; --j) arr[j] = arr[j - 1];
    arr[low] = target;
}

void reverse_array(std::vector<std::pair<int,int>>& arr, int left, int right) {
    right--;
    while (left < right) std::swap(arr[left++], arr[right--]);
}

int gallop_right(const std::vector<std::pair<int,int>>& arr, std::pair<int,int> key, int base, int length) {
    int ofs = 1, last_ofs = 0;
    while (ofs < length && arr[base + ofs] < key) {
        last_ofs = ofs;
        ofs = (ofs << 1) + 1;
        if (ofs <= 0) ofs = length;
    }
    if (ofs > length) ofs = length;
    int low = base + last_ofs, high = base + ofs;
    while (low < high) {
        int mid = (low + high) / 2;
        if (arr[mid] < key) low = mid + 1;
        else high = mid;
    }
    return low;
}

void galloping_merge(std::vector<std::pair<int,int>>& arr, int run1_start, int run1_len, int run2_start, int run2_len) {
    if ((int)temp.size() < run1_len) temp.resize(run1_len);
    for (int i = 0; i < run1_len; ++i) temp[i] = arr[run1_start + i];

    int i = 0, j = run2_start, dest = run1_start;
    int count1 = 0, count2 = 0;

    while (i < run1_len && j < run2_start + run2_len) {
        if (temp[i] <= arr[j]) {
            arr[dest++] = temp[i++];
            count1++;
            count2 = 0;
        } else {
            arr[dest++] = arr[j++];
            count2++;
            count1 = 0;
        }

        if (count1 >= GALLOP_THRESHOLD) {
            int gallop_idx = gallop_right(temp, arr[j], i, run1_len - i);
            while (i < gallop_idx) arr[dest++] = temp[i++];
            count1 = 0;
        } else if (count2 >= GALLOP_THRESHOLD) {
            int gallop_idx = gallop_right(arr, temp[i], j, run2_start + run2_len - j);
            while (j < gallop_idx) arr[dest++] = arr[j++];
            count2 = 0;
        }
    }
    while (i < run1_len) arr[dest++] = temp[i++];
    while (j < run2_start + run2_len) arr[dest++] = arr[j++];
}

void merge_with_stack(std::vector<std::pair<int,int>>& arr, std::vector<std::pair<int,int>>& run_list) {
    while (run_list.size() > 1) {
        int n = run_list.size();
        int A = (n >= 3) ? run_list[n-3].second : 0;
        int B = run_list[n-2].second;
        int C = run_list[n-1].second;
        if ((n >= 3 && A <= B + C) || (n >= 2 && B <= C)) {
            if (n >= 3 && A < C) {
                galloping_merge(arr, run_list[n-3].first, run_list[n-3].second,
                                     run_list[n-2].first, run_list[n-2].second);
                run_list[n-3].second += run_list[n-2].second;
                run_list.erase(run_list.begin() + n - 2);
            } else {
                galloping_merge(arr, run_list[n-2].first, run_list[n-2].second,
                                     run_list[n-1].first, run_list[n-1].second);
                run_list[n-2].second += run_list[n-1].second;
                run_list.pop_back();
            }
        } else {
            break;
        }
    }
}

void tim_sort(std::vector<std::pair<int,int>>& arr) {
    int n = arr.size();
    if (n <= 1) return;

    temp.resize(n);
    std::vector<std::pair<int,int>> run_arr;
    int i = 0;
    while (i < n) {
        int start = i;
        bool ascending = true;
        if (i < n - 1)
            ascending = (arr[i] <= arr[i + 1]);
        i++;

        if (ascending) {
            while (i < n && arr[i - 1] <= arr[i]) i++;
            int run_len = i - start;
            if (run_len < MIN_RUN) {
                int end = std::min(start + MIN_RUN, n);
                for (int j = start + 1; j < end; j++) binary_insertion(arr, start, j);
                i = end;
                run_len = i - start;
            }
            run_arr.emplace_back(start, run_len);
        } else {
            while (i < n && arr[i - 1] > arr[i]) i++;
            int run_len = i - start;
            if (run_len < MIN_RUN) {
                int end = std::min(start + MIN_RUN, n);
                for (int j = start + 1; j < end; j++) binary_insertion_reverse(arr, start, j);
                i = end;
                run_len = i - start;
            }
            reverse_array(arr, start, i);
            run_arr.emplace_back(start, run_len);
        }
        merge_with_stack(arr, run_arr);
    }

    while (run_arr.size() > 1) {
        galloping_merge(arr, run_arr[0].first, run_arr[0].second,
                             run_arr[1].first, run_arr[1].second);
        run_arr[0].second += run_arr[1].second;
        run_arr.erase(run_arr.begin() + 1);
    }
}
