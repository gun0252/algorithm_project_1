#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <climits>
#include <utility>

struct tree {
    std::pair<int,int> val;
    std::vector<tree*> child;
    tree(const std::pair<int,int>& v) : val(v), child() {}
};

void delete_tree(tree* node) {
    if (!node) return;
    for (tree* child : node->child) {
        delete_tree(child);
    }
    delete node;
}

// 두 트리를 비교해서 값이 작은 쪽이 승자가 되어, 승자의 child에 패자를 추가
tree* tournament_each(tree* a, tree* b) {
    if (a->val.first < b->val.first) { // a의 값이 b의 값보다 작으면 a가 승리
        a->child.push_back(b);
        return a;
    }
    else {
        b->child.push_back(a);
        return b;
    }
}

tree* tournament_build(const std::vector<tree*>& arr) {
    if (arr.empty())
        return nullptr;
    std::vector<tree*> current = arr;
    while (current.size() > 1) {
        std::vector<tree*> next;
        for (size_t i = 0; i < current.size(); i += 2) {
            if (i + 1 < current.size())
                next.push_back(tournament_each(current[i], current[i+1]));
            else
                next.push_back(current[i]);  // 홀수 개면 마지막 원소는 그대로 진출
        }
        current = next;
    }
    return current[0];
}

void tournament_sort(std::vector<std::pair<int,int>>& arr) {
    std::vector<tree*> forest;
    for (const auto &p : arr) {
        forest.push_back(new tree(p));
    }
    
    std::vector<std::pair<int,int>> sorted;
    tree* winner = nullptr;
    while (!forest.empty()) {
        tree* winner = tournament_build(forest);
        if (!winner)
            break;
        
        sorted.push_back(winner->val);
        forest = winner->child;
    }
    
    arr = sorted;
    delete_tree(winner);
}