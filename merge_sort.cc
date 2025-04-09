#include "sorting.h"
#include <vector>
#include <limits>
#include <utility>
static const int INF = std::numeric_limits<int>::max();
static void merge(std::pair<int,int>* arr, int p, int q, int r) {
    int n1 = q - p + 1;
    int n2 = r - q;

    std::vector<std::pair<int,int>> L(n1 + 1);
    std::vector<std::pair<int,int>> R(n2 + 1);

    for(int i = 0; i < n1; i++) L[i] = arr[p + i];
    for(int j = 0; j < n2; j++) R[j] = arr[q + 1 + j];

    L[n1] = {INF,INF};
    R[n2] = {INF,INF};

    int i = 0;
    int j = 0;
    for(int k = p; k <= r; k++) {
        if(L[i].first <= R[j].first) {
            arr[k] = L[i];
            i++;
        } 
        else {
            arr[k] = R[j];
            j++;
        }
    }
}

static void merge_sort_recursive(std::pair<int,int>* arr, int p, int r) {
    if(p<r){
        int q=(p+r)/2;
        merge_sort_recursive(arr, p, q);
        merge_sort_recursive(arr, q+1, r);
        merge(arr, p, q, r);
    }
}

void merge_sort(std::vector<std::pair<int,int>>& arr) {
    if(!arr.empty()){
        merge_sort_recursive(arr.data(), 0, arr.size()-1);
    }
}
