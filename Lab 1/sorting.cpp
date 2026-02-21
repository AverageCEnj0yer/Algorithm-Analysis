#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>

namespace MergeSort
{
void merge(int* arr, int* aux, size_t left, size_t middle, size_t right)
{
    for(size_t i {left}; i <= right; ++i)
        aux[i] = arr[i];
    
    size_t k {left};
    size_t i {left};
    size_t j {middle + 1};
    while(i <= middle && j <= right)
    {
        if (aux[i] <= aux[j])
            arr[k++] = aux[i++];
        else
            arr[k++] = aux[j++];
    }
    while(i <= middle)
    {
        arr[k++] = aux[i++];
    }
}
void mergeSort(int* arr, int* aux, size_t left, size_t right)
{
    if (left >= right)
        return;
    size_t middle = left + (right - left) / 2;
    mergeSort(arr, aux, left, middle);
    mergeSort(arr, aux, middle + 1, right);
    merge(arr, aux, left, middle, right);
}
void sort(int* arr, size_t left, size_t right)
{
    int* aux = new int[right + 1];
    mergeSort(arr, aux, 0, right);
    delete[] aux;

}
}
namespace QuickSort
{
// Manual sort for segments size 2 or 3 to protect Median-of-Three logic
void smallSort(int* arr, int low, int high) {
    int size = high - low + 1;
    if (size <= 1) return;
    if (size == 2) {
        if (arr[low] > arr[high]) std::swap(arr[low], arr[high]);
    } else if (size == 3) {
        if (arr[low] > arr[low + 1]) std::swap(arr[low], arr[low + 1]);
        if (arr[low] > arr[high]) std::swap(arr[low], arr[high]);
        if (arr[low + 1] > arr[high]) std::swap(arr[low + 1], arr[high]);
    }
}
int partition(int* arr, int low, int high) {
    int mid = low + (high - low) / 2;

    // 1. Median-of-Three (Sentinels)
    if (arr[mid] < arr[low]) std::swap(arr[mid], arr[low]);
    if (arr[high] < arr[low]) std::swap(arr[high], arr[low]);
    if (arr[high] < arr[mid]) std::swap(arr[high], arr[mid]);

    // 2. Hide pivot at high - 1
    std::swap(arr[mid], arr[high - 1]);
    int pivot = arr[high - 1];

    // 3. Pointers (i will start at low + 1, j at high - 2)
    int i = low; 
    int j = high - 1; 

    while (true) {
        // High-speed loops: no boundary checks needed
        while (arr[++i] < pivot); 
        while (arr[--j] > pivot);

        if (i >= j) break;
        std::swap(arr[i], arr[j]);
    }

    // 4. Restore pivot
    std::swap(arr[i], arr[high - 1]);
    return i;
}
void sort(int* arr, int low, int high) {
    while (low < high) {
        if (high - low < 3) {
            smallSort(arr, low, high);
            break;
        }

        int pi = partition(arr, low, high);

        // Tail Recursion Optimization
        if (pi - low < high - pi) {
            sort(arr, low, pi - 1);
            low = pi + 1;
        } else {
            sort(arr, pi + 1, high);
            high = pi - 1;
        }
    }
}
}
namespace HeapSort
{
    void heapify(int* arr, int i, int size)
    {
        int largest{i};

        int leftChild{i * 2 + 1};
        int rightChild {i * 2 + 2};

        if(leftChild < size && arr[leftChild] > arr[largest])
            largest = leftChild;
        if(rightChild < size && arr[rightChild] > arr[largest])
            largest = rightChild;
        if (largest != i)
        {
            std::swap(arr[i], arr[largest]);
            heapify(arr, largest, size);
        }
    }
    void sort(int* arr, int low, int high)
    {
        int size = high - low + 1;
        for (int i = size / 2 - 1; i >= 0; --i)
            heapify(arr, i, size);
        for(int i = size - 1; i > 0; --i)
        {
            std::swap(arr[0], arr[i]);
            heapify(arr, 0, i);
        }
    }
}
namespace BubbleSort
{
void sort(int* arr, int left, int right)
{
    for (int i = left; i < right; ++i)
    {
        bool swapped = false;
        for (int j = left; j < right - (i - left); ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}
}
//-----------Extras-------------
namespace SelectionSort
{
void sort(int* arr, int left, int right)
{
    for (int i = left; i < right; ++i)
    {
        int minIdx = i;
        for (int j = i + 1; j <= right; ++j)
        {
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        if (minIdx != i)
            std::swap(arr[i], arr[minIdx]);
    }
}
}
namespace InsertionSort
{
void sort(int* arr, int left, int right)
{
    for (int i = left + 1; i <= right; ++i)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}
}

//helper
void printArray(int* arr, int size)
{
    for(int i = 0; i < size; ++i)
    {   
        if (i > 0 && i % 10 == 0) std::cout << "\n";
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

int main()
{
    size_t size{};
    std::cin >> size;
    int* arr = new int[size];

    std::random_device rd{};
    std::seed_seq seq{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
    std::mt19937 mt{seq};

    std::uniform_int_distribution generate(0, 10/*std::numeric_limits<int>::max()*/);

    //warm up the random number generator
    for(int i = 0; i < 1000000; ++i)
        generate(mt);

    for(int i = 0; i < size; ++i)
        arr[i] = generate(mt);

    printArray(arr, size);
    //=============SORTIN AND TIME CALCULATION==================
    auto start = std::chrono::high_resolution_clock::now();
    HeapSort::sort(arr, 0, size - 1); //SORTING IS HERE
    auto end = std::chrono::high_resolution_clock::now();
    printArray(arr, size);
    std::cout << "\n";
    // Calculate duration in milliseconds
    std::chrono::duration<double, std::milli> duration = end - start;
    double ms = duration.count();

    std::cout << ms << " Miliseconds\n";


    delete[] arr;
}
