#include "sorting.h"
#include <limits>
#include <algorithm>
#include <vector>

const int INF = std::numeric_limits<int>::max();

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

    // Fill leaf nodes
    for (int i = 0; i < n; i++) {
        tree[offset + i] = arr[i];
    }

    // Build internal nodes
    for (int i = offset - 1; i >= 0; --i) {
        tree[i] = std::min(tree[2 * i + 1], tree[2 * i + 2]);
    }

    for (int i = 0; i < n; ++i) {
        std::pair<int,int> winner = tree[0];
        arr[i] = winner;

        // Find winner in leaf region (brute-force but safe)
        for (int j = 0; j < leaf_count; ++j) {
            if (tree[offset + j] == winner) {
                tree[offset + j] = {INF, INF};
                update_tree(tree, offset + j);
                break;
            }
        }
    }
}
