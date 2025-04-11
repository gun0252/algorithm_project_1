#ifndef SORTING_H
#define SORTING_H

#include <vector>
#include <string>

// ===== 정렬 알고리즘 함수 선언 =====

//conventional sorting algorithm
void insertion_sort(std::vector<std::pair<int,int>>& arr);
void selection_sort(std::vector<std::pair<int,int>>& arr);
void bubble_sort(std::vector<std::pair<int,int>>& arr);
void merge_sort(std::vector<std::pair<int,int>>& arr);
//void quick_sort(std::vector<std::pair<int,int>>& arr);
void heap_sort(std::vector<std::pair<int,int>>& arr);


//contemporary sorting algorithm
void tim_sort(std::vector<std::pair<int,int>>& arr);
void cocktail_shaker_sort(std::vector<std::pair<int,int>>& arr);
void library_sort(std::vector<std::pair<int,int>>& arr);
void comb_sort(std::vector<std::pair<int,int>>& arr);
void tournament_sort(std::vector<std::pair<int,int>>& arr);
void intro_sort(std::vector<std::pair<int,int>>& arr);

#endif // SORTING_H
