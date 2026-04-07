#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include "sorting.cpp"

void fill_random(int* arr, size_t size, std::mt19937& mt) {
    std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
    for (size_t i = 0; i < size; ++i)
        arr[i] = dist(mt);
}

// Wrappers for sort functions
void bubble_sort_wrapper(int* arr, size_t size) { BubbleSort::sort(arr, 0, static_cast<int>(size) - 1); }
void merge_sort_wrapper(int* arr, size_t size) { MergeSort::sort(arr, 0, static_cast<int>(size) - 1); }
void quick_sort_wrapper(int* arr, size_t size) { QuickSort::sort(arr, 0, static_cast<int>(size) - 1); }
void heap_sort_wrapper(int* arr, size_t size) { HeapSort::sort(arr, 0, static_cast<int>(size) - 1); }

void steroids_bubble_sort_wrapper(int* arr, size_t size) { SteroidsBubbleSort::sort(arr, 0, static_cast<int>(size) - 1); }
void steroids_merge_sort_wrapper(int* arr, size_t size) { SteroidsMergeSort::sort(arr, 0, static_cast<int>(size) - 1); }
void steroids_quick_sort_wrapper(int* arr, size_t size) { SteroidsQuickSort::sort(arr, 0, static_cast<int>(size) - 1); }
void steroids_heap_sort_wrapper(int* arr, size_t size) { SteroidsHeapSort::sort(arr, 0, static_cast<int>(size) - 1); }

struct Algo {
    const char* name;
    void (*sort_func)(int*, size_t);
};

int main() {
    std::random_device rd{};
    std::seed_seq seq{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
    std::mt19937 mt{seq};

    std::vector<size_t> sizes_large = {10, 100, 1000, 10000, 100000, 1000000, 10000000};
    std::vector<size_t> sizes_small = {10, 100, 1000, 10000};
    const int repeats = 5;

    std::vector<Algo> algos_small = {
        {"BubbleSort", bubble_sort_wrapper},
        {"SteroidsBubbleSort", steroids_bubble_sort_wrapper}
    };
    std::vector<Algo> algos_large = {
        {"MergeSort", merge_sort_wrapper},
        {"QuickSort", quick_sort_wrapper},
        {"HeapSort", heap_sort_wrapper},
        {"SteroidsMergeSort", steroids_merge_sort_wrapper},
        {"SteroidsQuickSort", steroids_quick_sort_wrapper},
        {"SteroidsHeapSort", steroids_heap_sort_wrapper}
    };

    std::string output_folder = "random_timing";
    system(("mkdir -p " + output_folder).c_str());

    for (const auto& algo : algos_small) {
        std::vector<double> averages;
        std::ofstream csv(output_folder + "/" + algo.name + ".csv");
        csv << "Size,Random\n";
        std::cout << "\n=== " << algo.name << " ===\n";
        std::cout << std::setw(12) << "Size" << std::setw(18) << "Random" << "\n";
        for (size_t size : sizes_small) {
            std::vector<int> arr(size);
            std::vector<int> arr_copy(size);
            double total = 0.0;
            for (int rep = 0; rep < repeats; ++rep) {
                fill_random(arr.data(), size, mt);
                std::copy(arr.begin(), arr.end(), arr_copy.begin());
                auto start = std::chrono::high_resolution_clock::now();
                algo.sort_func(arr_copy.data(), size);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> duration = end - start;
                total += duration.count();
            }
            double avg = total / repeats;
            averages.push_back(avg);
            csv << size << "," << avg << "\n";
            std::cout << std::setw(12) << size << std::setw(18) << std::fixed << std::setprecision(3) << avg << "\n";
        }
        csv.close();
    }

    for (const auto& algo : algos_large) {
        std::vector<double> averages;
        std::ofstream csv(output_folder + "/" + algo.name + ".csv");
        csv << "Size,Random\n";
        std::cout << "\n=== " << algo.name << " ===\n";
        std::cout << std::setw(12) << "Size" << std::setw(18) << "Random" << "\n";
        for (size_t size : sizes_large) {
            std::vector<int> arr(size);
            std::vector<int> arr_copy(size);
            double total = 0.0;
            for (int rep = 0; rep < repeats; ++rep) {
                fill_random(arr.data(), size, mt);
                std::copy(arr.begin(), arr.end(), arr_copy.begin());
                auto start = std::chrono::high_resolution_clock::now();
                algo.sort_func(arr_copy.data(), size);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> duration = end - start;
                total += duration.count();
            }
            double avg = total / repeats;
            averages.push_back(avg);
            csv << size << "," << avg << "\n";
            std::cout << std::setw(12) << size << std::setw(18) << std::fixed << std::setprecision(3) << avg << "\n";
        }
        csv.close();
    }
    return 0;
}
