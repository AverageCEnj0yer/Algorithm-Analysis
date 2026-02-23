#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include "sorting.cpp" 

// Helper to generate arrays
void fill_random_actual(int* arr, size_t size, int min_val, int max_val, std::mt19937& mt) {
    std::uniform_int_distribution<int> dist(min_val, max_val);
    for (size_t i = 0; i < size; ++i)
        arr[i] = dist(mt);
}

void fill_random(int* arr, size_t size, std::mt19937& mt) {
    fill_random_actual(arr, size, 0, std::numeric_limits<int>::max(), mt);
}

void fill_sorted_case(int* arr, size_t size, std::mt19937&) {
    for (size_t i = 0; i < size; ++i)
        arr[i] = static_cast<int>(i);
}

void fill_reverse_sorted_case(int* arr, size_t size, std::mt19937&) {
    for (size_t i = 0; i < size; ++i)
        arr[i] = static_cast<int>(size - i);
}

void fill_high_duplicates(int* arr, size_t size, std::mt19937& mt) {
    fill_random_actual(arr, size, 0, 100, mt);
}

// Timing helper
template<typename Func>
double time_sort(Func sort_func, int* arr, size_t size) {
    auto start = std::chrono::high_resolution_clock::now();
    sort_func(arr, size);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}


// Wrappers for sort functions to match signature
typedef void(*SortFunc)(int*, size_t);

void bubble_sort_wrapper(int* arr, size_t size) { BubbleSort::sort(arr, 0, static_cast<int>(size) - 1); }
void merge_sort_wrapper(int* arr, size_t size) { MergeSort::sort(arr, 0, static_cast<int>(size) - 1); }
void quick_sort_wrapper(int* arr, size_t size) { QuickSort::sort(arr, 0, static_cast<int>(size) - 1); }
void heap_sort_wrapper(int* arr, size_t size) { HeapSort::sort(arr, 0, static_cast<int>(size) - 1); }

void steroids_bubble_sort_wrapper(int* arr, size_t size) { SteroidsBubbleSort::sort(arr, 0, static_cast<int>(size) - 1); }
void steroids_merge_sort_wrapper(int* arr, size_t size) { SteroidsMergeSort::sort(arr, 0, static_cast<int>(size) - 1); }
void steroids_quick_sort_wrapper(int* arr, size_t size) { SteroidsQuickSort::sort(arr, 0, static_cast<int>(size) - 1); }
void steroids_heap_sort_wrapper(int* arr, size_t size) { SteroidsHeapSort::sort(arr, 0, static_cast<int>(size) - 1); }

struct TestCase {
    const char* name;
    void (*fill_func)(int*, size_t, std::mt19937&);
};


#include <fstream>
void run_and_print_and_save(const char* algoname, void(*sort_func)(int*, size_t), const std::vector<size_t>& sizes, const std::vector<TestCase>& cases, std::mt19937& mt, const char* csv_folder) {
    std::cout << "\n=== " << algoname << " ===\n";
    std::cout << std::left << std::setw(12) << "Size";
    for (const auto& tc : cases)
        std::cout << std::setw(18) << tc.name;
    std::cout << "\n";

    const int repeats = 5;
    std::vector<std::vector<double>> results(cases.size(), std::vector<double>(sizes.size()));

    for (size_t s = 0; s < sizes.size(); ++s) {
        size_t size = sizes[s];
        std::cout << std::left << std::setw(12) << size;
        std::vector<int> arr(size);
        std::vector<int> arr_copy(size);

        for (size_t t = 0; t < cases.size(); ++t) {
            const auto& tc = cases[t];
            double total = 0.0;
            for (int rep = 0; rep < repeats; ++rep) {
                tc.fill_func(arr.data(), size, mt);
                std::copy(arr.begin(), arr.end(), arr_copy.begin());
                total += time_sort(sort_func, arr_copy.data(), size);
            }
            double avg = total / repeats;
            results[t][s] = avg;
            std::cout << std::setw(18) << std::fixed << std::setprecision(3) << avg;
        }
        std::cout << "\n";
    }
    // Save to CSV for plotting
    std::ofstream csv(std::string(csv_folder) + "/" + algoname + ".csv");
    csv << "Size";
    for (const auto& tc : cases) csv << "," << tc.name;
    csv << "\n";
    for (size_t s = 0; s < sizes.size(); ++s) {
        csv << sizes[s];
        for (size_t t = 0; t < cases.size(); ++t) {
            csv << "," << results[t][s];
        }
        csv << "\n";
    }
    csv.close();
}


int main() {
    std::random_device rd{};
    std::seed_seq seq{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
    std::mt19937 mt{seq};

    std::vector<TestCase> cases = {
        {"Random", fill_random},
        {"Sorted", fill_sorted_case},
        {"Reverse Sorted", fill_reverse_sorted_case},
        {"High Duplicates", fill_high_duplicates},
    };

    std::vector<size_t> small_sizes = {10, 100, 1000, 10000};
    std::vector<size_t> large_sizes = {10, 100, 1000, 10000, 100000, 1000000, 10000000};

    // Normal algorithms
    run_and_print_and_save("BubbleSort", bubble_sort_wrapper, small_sizes, cases, mt, "diagrams");
    run_and_print_and_save("MergeSort", merge_sort_wrapper, large_sizes, cases, mt, "diagrams");
    run_and_print_and_save("QuickSort", quick_sort_wrapper, small_sizes, cases, mt, "diagrams");
    run_and_print_and_save("HeapSort", heap_sort_wrapper, large_sizes, cases, mt, "diagrams");

    // Steroids algorithms
    run_and_print_and_save("SteroidsBubbleSort", steroids_bubble_sort_wrapper, small_sizes, cases, mt, "diagrams_steroids");
    run_and_print_and_save("SteroidsMergeSort", steroids_merge_sort_wrapper, large_sizes, cases, mt, "diagrams_steroids");
    run_and_print_and_save("SteroidsQuickSort", steroids_quick_sort_wrapper, large_sizes, cases, mt, "diagrams_steroids");
    run_and_print_and_save("SteroidsHeapSort", steroids_heap_sort_wrapper, large_sizes, cases, mt, "diagrams_steroids");

    return 0;
}
