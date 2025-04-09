#include "sorting.h"
#include <limits>
#include <algorithm>
#include <vector>

const int INF = std::numeric_limits<int>::max();

// 리프 → 루트로 올라가면서 트리 갱신
void update_tree(std::vector<std::pair<int,int>>& tree, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        int left = 2 * parent + 1;
        int right = 2 * parent + 2;
        tree[parent] = std::min(tree[left], tree[right]);
        idx = parent;
    }
}

void tournament_sort(std::vector<std::pair<int,int>>& arr) {
    int n = arr.size();
    if (n == 0) return;

    int leaf_count = 1;
    while (leaf_count < n) leaf_count *= 2;
    int offset = leaf_count - 1;
    int tree_size = 2 * leaf_count - 1;

    std::vector<std::pair<int, int>> tree(tree_size, {INF, INF});

    // 리프에 값 채우기
    for (int i = 0; i < n; i++) {
        tree[offset + i] = arr[i];
    }

    // 초기 트리 구성 (bottom-up)
    for (int i = offset - 1; i >= 0; --i) {
        tree[i] = std::min(tree[2 * i + 1], tree[2 * i + 2]);
    }

    // 정렬 결과 채우기
    for (int i = 0; i < n; ++i) {
        std::pair<int,int> winner = tree[0];
        arr[i] = winner;

        // 리프에서 winner 위치 찾아서 제거
        int idx = -1;
        for (int j = 0; j < leaf_count; ++j) {
            if (tree[offset + j] == winner) {
                idx = offset + j;
                tree[idx] = {INF, INF};
                break;
            }
        }

        if (idx != -1) {
            update_tree(tree, idx);  // 해당 경로만 위로 갱신
        }
    }
}
