#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

vector<int> shuffled_sequence(int size, int start = 0) {
    vector<int> result(size);
    iota(result.begin(), result.end(), start);
    random_shuffle(result.begin(), result.end());
    return result;
}

// Быстрая сортировка (in-place)
int partition(vector<int>& arr, int start, int end) {
    int pivotIndex = start; // Выбор опорного элемента (можно менять на random)
    int pivot = arr[pivotIndex];

    int i = start + 1;
    int j = end;

    while (i <= j) {
        while (i <= end && arr[i] < pivot) {
            i++;
        }
        while (j >= start + 1 && arr[j] > pivot) {
            j--;
        }

        if (i < j) {
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[pivotIndex], arr[j]);
    return j;
}


void quickSort(vector<int>& arr, int start, int end) {
    int len = end - start + 1;
    if (len <= 2) {
        if (len == 2 && arr[start] > arr[end]) {
            swap(arr[start], arr[end]);
        }
        return;
    }

    int p = partition(arr, start, end);
    quickSort(arr, start, p - 1);
    quickSort(arr, p + 1, end);
}


int main() {
    const int size = 10000000;
    vector<int> arr;
    
    // Insert elements into container.
    const auto elems_to_add = shuffled_sequence(size);
    for (const auto &elem: elems_to_add) {
        arr.push_back(elem);
    }

    quickSort(arr, 0, arr.size() - 1);

    cout << "Отсортированный массив: ";
    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl;


    return 0;
}