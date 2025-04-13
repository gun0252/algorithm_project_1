#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <random>
#include <numeric>
#include <functional>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include "sorting.h"

using namespace std;
using namespace chrono;

// ===============
// 정렬 정확성 체크 함수(accuracy)
// ===============
bool is_sorted_correctly(const vector<pair<int, int>>& sorted) {
    int n = sorted.size();
    for (int i = 1; i < n; ++i) {
        if (sorted[i - 1].first > sorted[i].first) {
            return false;
        }
    }
    return true;
}

// ===============
// 안정성 체크 함수 (stablity)
// ===============
bool is_stable_linear(const vector<pair<int, int>>& sorted) {
    int n = sorted.size();
    for (int i = 1; i < n; ++i) {
        if (sorted[i].first == sorted[i - 1].first) {
            if (sorted[i].second < sorted[i - 1].second) {
                return false;
            }
        }
    }
    return true;
}

// ===============
// 시간 측정(time)
// ===============
double measure_time(const vector<pair<int,int>>& data, int trials,
                    void(*sorting_func)(vector<pair<int,int>>&)) {
    double total = 0.0;

    for(int t = 0; t < trials; t++) {
        vector<pair<int,int>> arr = data;

        auto start = high_resolution_clock::now();
        sorting_func(arr);
        auto end   = high_resolution_clock::now();

        total += duration<double>(end - start).count();
    }
    return total / trials;
}

// ===============
// result()
// ===============
void result(void(*sorting_func)(vector<pair<int,int>>&), const string& func_name){
    vector<int> sizes = {1000,10000,100000,1000000};
    int num_trials = 10;

    string filename = func_name;
    replace(filename.begin(), filename.end(), ' ', '_');
    ofstream fout(filename + ".txt");

    cout << "\n[" << func_name << " Performance Test]\n\n";
    fout << "\n[" << func_name << " Performance Test]\n\n";

    vector<pair<string, function<vector<pair<int,int>>(int)>>> inputs = {
        {
            "Sorted",
            [](int n){
                vector<pair<int,int>> v(n);
                for(int i = 0; i < n; i++) v[i] = {i, i};
                return v;
            }
        },
        {
            "Reversed",
            [](int n){
                vector<pair<int,int>> v(n);
                for(int i = 0; i < n; i++) v[i] = {n - i, i};
                return v;
            }
        },
        {
            "Random",
            [](int n){
                vector<pair<int,int>> v(n);
                mt19937 rng(random_device{}());
                uniform_int_distribution<int> dist(0, n - 1);
                for(int i = 0; i < n; i++) v[i] = {dist(rng), i};
                return v;
            }
        },
        {
            "Duplicate-heavy",
            [](int n){
                vector<pair<int,int>> v(n);
                mt19937 rng(random_device{}());
                uniform_int_distribution<int> dist(0, 10); // 많은 중복을 유도
                for (int i = 0; i < n; ++i) {
                    v[i] = {dist(rng), i};  // 중복 값들, index는 고유
                }
                return v;
            }
        },
        {
            "Nearly Sorted",
            [](int n){
                vector<pair<int,int>> v(n);
                for (int i = 0; i < n; ++i)
                    v[i] = {i, i}; // 완전 정렬
        
                // 일부를 섞어줌 (1~5%만 swap)
                mt19937 rng(random_device{}());
                int swaps = max(1, n / 50); // n의 2% 정도
        
                for (int i = 0; i < swaps; ++i) {
                    int a = rng() % n;
                    int b = rng() % n;
                    swap(v[a], v[b]);
                }
                return v;
            }
        }
    };
    
    for(auto &inp : inputs){
        auto label = inp.first;
        auto generator = inp.second;

        cout << "[" << label << " Input]\n";
        fout << "[" << label << " Input]\n";

        for(auto n : sizes){
            cout << "n = " << n << endl;
            fout << "n = " << n << endl;

            auto data = generator(n);
            double avg_time = measure_time(data, num_trials, sorting_func);

            cout << "  Average: " << avg_time << "s\n";
            fout << "  Average: " << avg_time << "s\n";

            vector<pair<int,int>> userSorted = data;
            sorting_func(userSorted);

            bool sorted_ok = is_sorted_correctly(userSorted);
            cout << "    Sorted Correctly? " << (sorted_ok ? "Yes" : "No") << "\n";
            fout << "    Sorted Correctly? " << (sorted_ok ? "Yes" : "No") << "\n";

            if (label == "Random") {
                bool stable_result = is_stable_linear(userSorted);
                cout << "    Stable? " << (stable_result ? "Yes" : "No") << "\n";
                fout << "    Stable? " << (stable_result ? "Yes" : "No") << "\n";
            }

            cout << "\n";
            fout << "\n";
        }
    }

    fout.close();
}

// ===============
// main
// ===============
int main(int argc, char* argv[]) {
    vector<pair<string, void(*)(vector<pair<int,int>>&)> > sorters = {
        //{"insertion", insertion_sort},
        //{"selection", selection_sort},
        //{"bubble",    bubble_sort},
        {"quick",     quick_sort},
        {"merge",     merge_sort},
        {"heap",      heap_sort},
        {"intro",     intro_sort},
        {"tim",       tim_sort},
        {"tournament",tournament_sort},
        {"cocktail_shaker", cocktail_shaker_sort},
        {"comb",      comb_sort},
        {"library", library_sort},
        {"rand", quick_sort_random}
    };

    if (argc == 1 || string(argv[1]) == "all") {
        for (auto& sorter : sorters) {
            result(sorter.second, sorter.first + " Sort");
        }
    } else {
        string target = argv[1];
        for (auto& sorter : sorters) {
            if (sorter.first == target) {
                result(sorter.second, sorter.first + " Sort");
                return 0;
            }
        }
    }

    return 0;
}
