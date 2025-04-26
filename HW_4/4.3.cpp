#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace chrono;

// Быстрая сортировка (QuickSort)
void quickSortStandard(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (arr[j] < pivot) swap(arr[++i], arr[j]);
        }
        swap(arr[i + 1], arr[high]);
        int p = i + 1;
        quickSortStandard(arr, low, p - 1);
        quickSortStandard(arr, p + 1, high);
    }
}

// Сортировка вставками (Insertion Sort)
void insertionSort(vector<int>& arr, int l, int r) {
    for (int i = l + 1; i <= r; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= l && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// Гибрид QuickSort + Insertion Sort
// - Если размер подмассива ≤ CUT_OFF, используем Insertion Sort для оптимизации малых участков.
// - Иначе – классический QuickSort.
void quickSortHybrid(vector<int>& arr, int low, int high) {
    const int CUT_OFF = 10;
    if (high - low + 1 <= CUT_OFF) {
        insertionSort(arr, low, high);
    } else if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (arr[j] < pivot) swap(arr[++i], arr[j]);
        }
        swap(arr[i + 1], arr[high]);
        int p = i + 1;
        quickSortHybrid(arr, low, p - 1);
        quickSortHybrid(arr, p + 1, high);
    }
}

// Генерация случайного массива заданного размера
vector<int> generateRandomArray(int size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100000);
    vector<int> a(size);
    for (int& x : a) x = dis(gen);
    return a;
}

// Замер времени выполнения сортировки в микросекундах (милли оказалось недостаточно)
// - sortFunc: указатель на функцию сортировки
// - Возвращает длительность в мкс 
long long measureTime(void (*sortFunc)(vector<int>&, int, int), vector<int> arr) {
    auto start = high_resolution_clock::now();
    sortFunc(arr, 0, arr.size() - 1);
    auto stop  = high_resolution_clock::now();
    return duration_cast<microseconds>(stop - start).count();
}

int main() {
    vector<int> sizes = {1000, 5000, 10000};
    for (int n : sizes) {
        cout << "Массив размером " << n << " элементов" << endl;
        auto original = generateRandomArray(n);

        // Копируем массивы для двух вариантов сортировки
        auto a1 = original;
        auto a2 = original;

        // Замеры времени
        long long tStandard = measureTime(quickSortStandard, a1);
        long long tHybrid   = measureTime(quickSortHybrid,   a2);

        cout << "  Стандартная QuickSort: " << tStandard << " мкс" << endl;
        cout << "  Гибридная Quick+Insertion: " << tHybrid   << " мкс" << endl;
        cout << "───･ﾟ✧*･ﾟ✧*･ﾟ✧*･ﾟ✧*───" << endl;
    }
    return 0;
}
