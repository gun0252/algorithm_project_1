#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

static const double EPSILON = 0.5;
static const int EMPTY = std::numeric_limits<int>::min();

int round_number(int n) {
    int i = 1, k = 0;
    while (i < n) {
        i <<= 1;
        k++;
    }
    return k;
}

int rebalancing(std::vector<std::pair<int,int>>& arr, std::vector<bool>& occ ,int cur_size, int max_size, int cur_inserted) {
    int size_after_rebalancing = (cur_size == 1) 
                                 ? int((1 + EPSILON) * 2) 
                                 : cur_size * 2;
    if (size_after_rebalancing > max_size) {
        size_after_rebalancing = max_size;
    }

    std::vector<std::pair<int,int>> rebalanced(size_after_rebalancing, {EMPTY,EMPTY});
    std::vector<bool> new_occ(size_after_rebalancing, false);

    int idx = 1;
    int gap = (cur_inserted == 0) ? 1 : size_after_rebalancing / cur_inserted; 
    if (gap < 1) gap = 1;

    for (int i = 0; i < cur_size; i++) {
        if (occ[i]) {
            if (idx >= size_after_rebalancing) break;
            rebalanced[idx] = arr[i];
            new_occ[idx] = true;
            idx += gap;
        } 
    }

    for (int i = 0; i < size_after_rebalancing; i++) {
        arr[i] = rebalanced[i];
        occ[i] = new_occ[i];
    }

    return size_after_rebalancing;
}

int binary_search(const std::vector<std::pair<int,int>>& arr, int cur_size, const std::vector<bool>& occ, int target) {
    std::vector<int> occupied_idx;
    occupied_idx.reserve(cur_size);

    for (int i = 0; i < cur_size; i++) {
        if (occ[i]) occupied_idx.push_back(i);
    }

    int low = 0;
    int high = (int)occupied_idx.size() - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int val = arr[occupied_idx[mid]].first;
        if (val == target) {
            return occupied_idx[mid];
        }
        else if (val < target) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    if (low >= (int)occupied_idx.size()) {
        return occupied_idx.back(); 
    } 
    else {
        return occupied_idx[low];
    }
}

int insert_shift_left_or_right(std::vector<std::pair<int,int>>& arr,
                               std::pair<int,int> incremental_elem,
                               int index,
                               std::vector<bool>& occ,
                               int max_size) 
{
    if (index == -1) {
        int cur = 0;
        while (cur < max_size && occ[cur])
            cur++;
        if (cur == max_size) return -1;
        for (int i = cur; i > 0; i--) {
            arr[i] = arr[i - 1];
            occ[i] = occ[i - 1];
        }
        arr[0] = incremental_elem;
        occ[0] = true;
        return 0;
    }
    else if (index == max_size) {
        int cur = max_size - 1;
        while (cur >= 0 && occ[cur])
            cur--;
        if (cur < 0) return -1;
        for (int i = cur; i < max_size - 1; i++) {
            arr[i] = arr[i + 1];
            occ[i] = occ[i + 1];
        }
        arr[max_size - 1] = incremental_elem;
        occ[max_size - 1] = true;
        return max_size - 1;
    }
    else {
        int right = index;
        while (right < max_size && occ[right]) right++;
        int left = index;
        while (left >= 0 && occ[left]) left--;

        bool right_free = (right < max_size);
        bool left_free = (left >= 0);

        int right_gap = right_free ? (right - index) : max_size;
        int left_gap  = left_free  ? (index - left) : max_size;

        if (right_free && (!left_free || right_gap <= left_gap)) {
            for (int i = right; i > index; i--) {
                arr[i] = arr[i - 1];
                occ[i] = occ[i - 1];
            }
            arr[index] = incremental_elem;
            occ[index] = true;
            return index;
        } 
        else if (left_free) {
            for (int i = left + 1; i <= index; i++) {
                arr[i - 1] = arr[i];
                occ[i - 1] = occ[i];
            }
            arr[index - 1] = incremental_elem;
            occ[index - 1] = true;
            return index - 1;
        } else {
            return -1;
        }
    }
}

void insert_incremental(std::vector<std::pair<int,int>>& arr,
                        std::pair<int,int> incremental_elem,
                        int index,
                        std::vector<bool>& occ,
                        int max_size)
{
    if (index < max_size && index > -1 && arr[index].first == EMPTY) {
        arr[index] = incremental_elem;
        occ[index] = true;
    }
    else {
        insert_shift_left_or_right(arr, incremental_elem, index, occ, max_size);
    }
}

void library_sort(std::vector<std::pair<int,int>>& arr) {
    int n = (int)arr.size();
    int new_array_size = (int)std::ceil((1 + EPSILON) * n);

    std::vector<std::pair<int,int>> new_space(new_array_size, {EMPTY, EMPTY});
    std::vector<bool> occ(new_array_size, false);

    int total_round = round_number(n);

    new_space[0] = arr[0];
    occ[0] = true;

    int cur_idx = 1;
    int cur_size = 1;
    int cur_inserted = 1;

    for (int cur_round = 1; cur_round <= total_round; cur_round++) {
        cur_size = rebalancing(new_space, occ, cur_size, new_array_size, cur_inserted);

        for (int j = 0; j < cur_inserted && cur_idx < n; j++) {
            int val = arr[cur_idx].first;
            int idx_found = binary_search(new_space, cur_size, occ, val);

            if (new_space[idx_found].first > val) {
            } 
            else if (new_space[idx_found].first < val) {
                idx_found++;
            } 
            else {
                idx_found++;
            }

            if (idx_found < -1) idx_found = -1;
            if (idx_found > cur_size) idx_found = cur_size;

            insert_incremental(new_space, arr[cur_idx], idx_found, occ, cur_size);
            cur_idx++;
        }

        cur_inserted *= 2;
    }

    int k = 0;
    for (int i = 0; i < cur_size; i++) {
        if (new_space[i].first != EMPTY) {
            arr[k++] = new_space[i];
        }
    }
}
