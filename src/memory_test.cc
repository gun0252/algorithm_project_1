#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#endif

#include "sorting.h" // 사용자가 정의한 정렬 함수 헤더

// OS에 따라 피크 메모리 측정 함수
size_t get_peak_memory_kb() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS memInfo;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo))) {
        return memInfo.PeakWorkingSetSize / 1024; // bytes -> KB
    }
    return 0;
#else
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // KB (Linux), macOS는 byte일 수 있음
#endif
}

int main(int argc, char* argv[]) {
    std::string sort_name = "sorting";
    if (argc >= 2) {
        sort_name = argv[1];
    }

    size_t start_memory, vector_memory, sort_memory;

    start_memory = get_peak_memory_kb();

    //const int n = 100000;
    const int n = 10000;
    //const int n = 1000;
    std::vector<int> raw(n);
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, n);
    for (int i = 0; i < n; ++i) {
        raw[i] = dist(rng);
    }

    vector_memory = get_peak_memory_kb();

    std::vector<std::pair<int, int>> data(n);
    for (int i = 0; i < n; ++i) {
        data[i] = {raw[i], i};
    }

    // 정렬 함수 교체 가능 (예: insertion_sort, merge_sort 등)
    if (sort_name == "insertion") insertion_sort(data);
    else if (sort_name == "selection") selection_sort(data);
    else if (sort_name == "bubble") bubble_sort(data);
    else if (sort_name == "quick") quick_sort(data);
    else if (sort_name == "merge") merge_sort(data);
    else if (sort_name == "heap") heap_sort(data);
    else if (sort_name == "intro") intro_sort(data);
    else if (sort_name == "tim") tim_sort(data);
    else if (sort_name == "tournament") tournament_sort(data);
    else if (sort_name == "cocktail_shaker") cocktail_shaker_sort(data);
    else if (sort_name == "comb") comb_sort(data);
    else if (sort_name == "library") library_sort(data);
    else {
        std::sort(data.begin(), data.end()); // fallback
    }

    sort_memory = get_peak_memory_kb();

    std::ofstream fout(sort_name + "_memory_10000.txt");
    fout << "[" << sort_name << " Performance Test]\n\n";
    fout << "Vector Allocation: " << vector_memory - start_memory << " KB\n";
    fout << "Sort Execution:   " << sort_memory - vector_memory << " KB\n";
    fout.close();

    std::cout << "Vector Allocation: " << vector_memory - start_memory << " KB\n";
    std::cout << "Sort Execution:   " << sort_memory - vector_memory << " KB\n";

    return 0;
}