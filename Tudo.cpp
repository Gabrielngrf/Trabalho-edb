#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <functional>
#include <map>


struct Metrics {
    long long comparisons = 0;
    long long swaps = 0;
    double time_ms = 0.0;
};


void bubbleSort(std::vector<int>& arr, Metrics& metrics);
void insertionSort(std::vector<int>& arr, Metrics& metrics);
void selectionSort(std::vector<int>& arr, Metrics& metrics);
void mergeSort(std::vector<int>& arr, Metrics& metrics);
void quickSort(std::vector<int>& arr, Metrics& metrics);




void bubbleSort(std::vector<int>& arr, Metrics& metrics) {
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            metrics.comparisons++;
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                metrics.swaps++;
                swapped = true;
            }
        }
        if (!swapped) break; 
    }
}


void insertionSort(std::vector<int>& arr, Metrics& metrics) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && (++metrics.comparisons, arr[j] > key)) {
            arr[j + 1] = arr[j];
            metrics.swaps++; 
            j--;
        }
        arr[j + 1] = key;
    }
}


void selectionSort(std::vector<int>& arr, Metrics& metrics) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            metrics.comparisons++;
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            std::swap(arr[i], arr[min_idx]);
            metrics.swaps++;
        }
    }
}


void merge(std::vector<int>& arr, int l, int m, int r, Metrics& metrics) {
    int n1 = m - l + 1;
    int n2 = r - m;
    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        metrics.comparisons++;
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSortRecursive(std::vector<int>& arr, int l, int r, Metrics& metrics) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortRecursive(arr, l, m, metrics);
        mergeSortRecursive(arr, m + 1, r, metrics);
        merge(arr, l, m, r, metrics);
    }
}

void mergeSort(std::vector<int>& arr, Metrics& metrics) {
    metrics.swaps = 0; 
    mergeSortRecursive(arr, 0, arr.size() - 1, metrics);
}


int partition(std::vector<int>& arr, int low, int high, Metrics& metrics) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; ++j) {
        metrics.comparisons++;
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
            metrics.swaps++;
        }
    }
    std::swap(arr[i + 1], arr[high]);
    metrics.swaps++;
    return (i + 1);
}

void quickSortRecursive(std::vector<int>& arr, int low, int high, Metrics& metrics) {
    if (low < high) {
        int pi = partition(arr, low, high, metrics);
        quickSortRecursive(arr, low, pi - 1, metrics);
        quickSortRecursive(arr, pi + 1, high, metrics);
    }
}

void quickSort(std::vector<int>& arr, Metrics& metrics) {
    quickSortRecursive(arr, 0, arr.size() - 1, metrics);
}



std::vector<int> generateRandomVector(int size) {
    std::vector<int> vec(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, size * 10);
    for (int i = 0; i < size; ++i) vec[i] = distrib(gen);
    return vec;
}

std::vector<int> generateNearlySortedVector(int size) {
    std::vector<int> vec(size);
    for (int i = 0; i < size; ++i) vec[i] = i + 1;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, size - 1);
    
   
    int swaps = size * 0.1;
    for (int i = 0; i < swaps; ++i) {
        std::swap(vec[distrib(gen)], vec[distrib(gen)]);
    }
    return vec;
}

std::vector<int> generateReverseOrderedVector(int size) {
    std::vector<int> vec(size);
    for (int i = 0; i < size; ++i) {
        vec[i] = size - i;
    }
    return vec;
}




int main() {
    std::vector<int> vector_sizes = {100, 1000, 5000};
    
    std::map<std::string, std::function<void(std::vector<int>&, Metrics&)>> algorithms;
    algorithms["Bubble Sort"] = bubbleSort;
    algorithms["Insertion Sort"] = insertionSort;
    algorithms["Selection Sort"] = selectionSort;
    algorithms["Merge Sort"] = mergeSort;
    algorithms["Quick Sort"] = quickSort;

    for (int size : vector_sizes) {
        std::cout << "\n==============================\n";
        std::cout << "Tamanho do vetor: " << size << "\n";
        std::cout << "==============================\n";

        for (int type_idx = 0; type_idx < 3; ++type_idx) {
            std::string type_name;
            std::vector<int> base_vector;

            switch (type_idx) {
                case 0:
                    type_name = "Aleatório";
                    base_vector = generateRandomVector(size);
                    break;
                case 1:
                    type_name = "Quase Ordenado";
                    base_vector = generateNearlySortedVector(size);
                    break;
                case 2:
                    type_name = "Reverso";
                    base_vector = generateReverseOrderedVector(size);
                    break;
            }

            std::cout << "\n--- Tipo de vetor: " << type_name << " ---\n";

            for (auto const& pair : algorithms) {
                std::string name = pair.first;
                auto func = pair.second;

                
                if ((name == "Bubble Sort" || name == "Selection Sort" || name == "Insertion Sort") && size > 10000)
                    continue;

                std::vector<int> to_sort = base_vector;
                Metrics m = {0, 0, 0.0};

                auto start = std::chrono::high_resolution_clock::now();
                func(to_sort, m);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> duration = end - start;
                m.time_ms = duration.count();

                std::cout << "\n=== " << name << " ===\n";
                std::cout << "Tempo de execução: " << m.time_ms << " ms\n";
                std::cout << "Comparações: " << m.comparisons << "\n";
                std::cout << "Trocas: " << m.swaps << "\n";
            }

            
            std::vector<int> to_sort_std = base_vector;
            auto start_std = std::chrono::high_resolution_clock::now();
            std::sort(to_sort_std.begin(), to_sort_std.end());
            auto end_std = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration_std = end_std - start_std;

            std::cout << "\n=== std::sort (referência) ===\n";
            std::cout << "Tempo de execução: " << duration_std.count() << " ms\n";
            std::cout << "Comparações: N/A\n";
            std::cout << "Trocas: N/A\n";
            std::cout << "===============================\n";
        }
    }

    return 0;
}
