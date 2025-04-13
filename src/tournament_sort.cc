#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <climits>
#include <utility>

// N-ary 트리 노드 정의 (val 타입을 std::pair<int,int>로 변경)
struct tree {
    std::pair<int,int> val;
    std::vector<tree*> child;
    tree(const std::pair<int,int>& v) : val(v), child() {}
};

// 후위 순회 방식으로 트리 전체 메모리 해제
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

// 주어진 forest (tree* 벡터)를 인접한 두 개씩 병합하여
// 최종적으로 하나의 승자 트리로 만드는 함수
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

// Tournament Sort 함수
// 입력 벡터의 각 pair를 동적할당된 tree의 leaf로 구성하고,
// 매 라운드 tournament_build를 실행하여 승자 트리의 루트가 정렬 결과를 형성하도록 한다.
// 승자의 child 벡터를 다음 라운드 forest로 넘겨준다.
void tournament_sort(std::vector<std::pair<int,int>>& arr) {
    std::vector<tree*> forest;
    // 초기 forest: 배열의 각 pair에 대해 새 leaf 노드 생성
    for (const auto &p : arr) {
        forest.push_back(new tree(p));
    }
    
    std::vector<std::pair<int,int>> sorted;
    tree* winner = nullptr;
    while (!forest.empty()) {
        tree* winner = tournament_build(forest);
        if (!winner)
            break;
        
        // 이번 라운드 승자의 루트가 최소값이 됨
        sorted.push_back(winner->val);
        
        // 다음 라운드의 forest는 승자 트리의 child 벡터 그대로 사용
        // 승자 트리가 리프 노드라면 child가 빈 벡터이므로 while 루프 종료
        forest = winner->child;
    }
    
    arr = sorted;
    delete_tree(winner);
    // 메모리 해제: 실제 사용 시, 동적 할당한 모든 tree*에 대해 delete 처리가 필요합니다.
}