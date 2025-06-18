#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <ctime>

using namespace std;
using namespace std::chrono;

// ========== Bubble Sort ==========
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// ========== Merge Sort ==========
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Wrapper for mergeSort
void mergeSortWrapper(vector<int>& arr) {
    mergeSort(arr, 0, arr.size() - 1);
}

// ========== Quick Sort ==========
int partition(vector<int>& arr, int low, int high) {
    int pivot_idx = low + rand() % (high - low + 1);
    swap(arr[pivot_idx], arr[high]);
    int pivot = arr[high];

    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Wrapper for quickSort
void quickSortWrapper(vector<int>& arr) {
    quickSort(arr, 0, arr.size() - 1);
}

// ========== Generate Data ==========
vector<int> generate_data(int size, const string& type) {
    vector<int> data(size);
    iota(data.begin(), data.end(), 1);

    random_device rd;
    mt19937 g(rd());

    if (type == "random") {
        shuffle(data.begin(), data.end(), g);
    } else if (type == "reverse") {
        reverse(data.begin(), data.end());
    } else if (type == "partial") {
        shuffle(data.begin(), data.begin() + size / 4, g);
    }

    return data;
}

// ========== Measure Time ==========
double measure_time(void (*sort_func)(vector<int>&), vector<int> data) {
    auto start = high_resolution_clock::now();
    sort_func(data);
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count() / 1000.0;
}

// ========== Main ==========
int main() {
    srand(time(0));
    vector<int> sizes = {100, 500, 1000, 5000, 10000};
    vector<string> types = {"random", "reverse", "partial"};

    for (int size : sizes) {
        cout << "\n=== Data Size: " << size << " ===" << endl;
        for (const string& type : types) {
            vector<int> data = generate_data(size, type);

            // Bubble Sort
            vector<int> copy = data;
            double time_bubble = measure_time(bubbleSort, copy);

            // Merge Sort
            copy = data;
            double time_merge = measure_time(mergeSortWrapper, copy);

            // Quick Sort
            copy = data;
            double time_quick = measure_time(quickSortWrapper, copy);

            cout << "\n[" << type << "]" << endl;
            cout << "Bubble: " << time_bubble << " ms" << endl;
            cout << "Merge: " << time_merge << " ms" << endl;
            cout << "Quick: " << time_quick << " ms" << endl;
        }
    }

    return 0;
}
