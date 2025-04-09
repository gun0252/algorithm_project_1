#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>
#include <stack>
#include <limits>

static const int MIN_RUN = 32;
static const int GALLOC_THRESHOLD = 7;
static const int INF = std::numeric_limits<int>::max();

// 삽입정렬: [left, right) 구간을 정렬 (구현되어 있다고 가정)
void insertion_sort_interval(std::vector<std::pair<int,int>>& arr, int left, int right);

void reverse_array(std::vector<std::pair<int,int>>& arr, int left, int right) {
    // right 는 exclusive bound
    right--; // 실제 마지막 원소 인덱스
    while (left < right) {
        std::swap(arr[left], arr[right]);
        left++;
        right--;
    }
}

int galloping_search(std::vector<std::pair<int,int>>& arr, int base, std::pair<int,int> target, bool isLeft) {
    int n = arr.size();
    int i = 1;
    // isLeft == true: base부터 오른쪽으로 탐색
    if (isLeft) {
        while (base + i < n && arr[base + i] < target)
            i <<= 1;
        int low = base + (i >> 1);
        int high = std::min(base + i, n - 1);
        while (low <= high) {
            int mid = (low + high) / 2;
            if (arr[mid] < target)
                low = mid + 1;
            else
                high = mid - 1;
        }
        return low;
    } 
    else {
        // isLeft == false: 반대방향 탐색 (필요 시 사용)
        while (base + i < n && target < arr[base + i])
            i <<= 1;
        int low = base + (i >> 1);
        int high = std::min(base + i, n - 1);
        while (low <= high) {
            int mid = (low + high) / 2;
            if (target < arr[mid])
                low = mid + 1;
            else
                high = mid - 1;
        }
        return low;
    }
}

void merge_array_with_galloping(std::vector<std::pair<int,int>>& arr, std::pair<int,int>& run1, std::pair<int,int>& run2) {
    // run1 : [run1.first, run1.first + run1.second)
    // run2 : [run2.first, run2.first + run2.second)
    std::vector<std::pair<int,int>> L(arr.begin() + run1.first, arr.begin() + run1.first + run1.second);
    std::vector<std::pair<int,int>> R(arr.begin() + run2.first, arr.begin() + run2.first + run2.second);
    
    // Sentinel로 INF를 추가 (비교 시 항상 마지막에 오도록)
    L.push_back({INF, INF});
    R.push_back({INF, INF});
    
    int i = 0, j = 0;
    int k = run1.first;  // 병합된 결과의 시작 위치

    int run1_galloping = 0, run2_galloping = 0;
    int totalL = run1.second, totalR = run2.second;
    
    // 두 run이 모두 다 처리될 때까지
    while (i < totalL || j < totalR) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
            run1_galloping++;
            run2_galloping = 0;
        } else {
            arr[k++] = R[j++];
            run2_galloping++;
            run1_galloping = 0;
        }
        
        // Galloping 모드: 한쪽 run이 연속해서 선택되면 한 번에 복사
        if (run1_galloping >= GALLOC_THRESHOLD) {
            int idx = galloping_search(L, i, R[j], true);
            while (i < idx && k < static_cast<int>(arr.size())) {
                arr[k++] = L[i++];
            }
            run1_galloping = 0;
        } else if (run2_galloping >= GALLOC_THRESHOLD) {
            int idx = galloping_search(R, j, L[i], true);
            while (j < idx && k < static_cast<int>(arr.size())) {
                arr[k++] = R[j++];
            }
            run2_galloping = 0;
        }
    }
}

// tim_sort_interval: 정렬할 배열의 [start, end) 구간에서 TimSort 수행
void tim_sort_interval(std::vector<std::pair<int,int>>& array, int start, int end) {
    // end는 exclusive bound, 즉 배열 전체 구간은 [0, array.size())
    std::vector<std::pair<int,int>> run_list;
    int i = start;
    while (i < end) {
        int run_start = i;
        // run 길이가 1이면 바로 처리
        if (i < end - 1) {
            bool ascending = true;
            if (array[i] > array[i+1])
                ascending = false;
            
            if (ascending) {
                while (i < end - 1 && array[i] <= array[i+1])
                    i++;
                // run은 run_start ~ i (양 끝 포함)
                if (i - run_start + 1 < MIN_RUN) {
                    int new_end = std::min(run_start + MIN_RUN, end);
                    insertion_sort_interval(array, run_start, new_end);
                    i = new_end - 1;
                }
            } else {  // 내림차순이면 run을 뒤집어서 오름차순으로
                while (i < end - 1 && array[i] > array[i+1])
                    i++;
                reverse_array(array, run_start, i+1);
                if (i - run_start + 1 < MIN_RUN) {
                    int new_end = std::min(run_start + MIN_RUN, end);
                    insertion_sort_interval(array, run_start, new_end);
                    i = new_end - 1;
                }
            }
        }
        // run의 길이는 최소 1 (run_start부터 i까지, 양 끝 포함)
        run_list.push_back({run_start, i - run_start + 1});
        i++;
    }
    
    // **Run 병합**  
    // TimSort의 invariant(불변식)를 단순화하여, 아래와 같이 merging 조건을 적용합니다.
    // pending run들을 vector에 저장한 후, invariant 위반 시 적절히 병합합니다.
    while (run_list.size() > 1) {
        int n_runs = run_list.size();
        bool merged = false;
        if (n_runs >= 3) {
            int A = run_list[n_runs-3].second;
            int B = run_list[n_runs-2].second;
            int C = run_list[n_runs-1].second;
            // invariant 위반 시 (즉, A <= B + C 또는 B <= C 인 경우)
            if (A <= B + C || B <= C) {
                // 두 run 중 길이가 더 작은 쪽을 병합
                if (B < C) {
                    // merge run_list[n_runs-3]과 run_list[n_runs-2]
                    merge_array_with_galloping(array, run_list[n_runs-3], run_list[n_runs-2]);
                    std::pair<int,int> merged_run = { run_list[n_runs-3].first,
                        run_list[n_runs-3].second + run_list[n_runs-2].second };
                    // 해당 두 run을 제거하고, merged_run을 삽입
                    run_list.erase(run_list.end()-2, run_list.end()-1); // run_list[n_runs-2] 제거
                    run_list[n_runs-3] = merged_run; // 업데이트
                    merged = true;
                } else {
                    // merge run_list[n_runs-2]와 run_list[n_runs-1]
                    merge_array_with_galloping(array, run_list[n_runs-2], run_list[n_runs-1]);
                    std::pair<int,int> merged_run = { run_list[n_runs-2].first,
                        run_list[n_runs-2].second + run_list[n_runs-1].second };
                    run_list[n_runs-2] = merged_run;
                    run_list.pop_back();
                    merged = true;
                }
            }
        }
        if (!merged) {
            // invariant 위반 조건이 없으면, 마지막 두 run을 병합
            int n_runs = run_list.size();
            merge_array_with_galloping(array, run_list[n_runs-2], run_list[n_runs-1]);
            std::pair<int,int> merged_run = { run_list[n_runs-2].first,
                run_list[n_runs-2].second + run_list[n_runs-1].second };
            run_list[n_runs-2] = merged_run;
            run_list.pop_back();
        }
    }
}

// 최종 TimSort 인터페이스: 배열 전체를 정렬합니다.
void tim_sort(std::vector<std::pair<int,int>>& array) {
    // end를 array.size()로 전달 (exclusive bound)
    tim_sort_interval(array, 0, array.size());
}
