#include <iostream>
#include <vector>
#include <algorithm> // чтобы юзать swap для обменом элементами
#include <chrono> // чтобы мерить время исполнения программы 
#include <random> // для генерации случайных элементов массива
#include <string>

using namespace std;
using namespace chrono;

// Обычная сортировка пузырьком 
void bubbleSortStandard(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Оптимизированная сортировка пузырьком
// + Проверка: если за один проход не было обменов, массив уже отсортирован
void bubbleSortOptimized(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        bool swapped = false; // Флаг обмена
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        // Если не было ни одного обмена — массив отсортирован, можно выйти
        if (!swapped) break;
    }
}

// Генерация случайного массива заданного размера
vector<int> generateRandomArray(int size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100000);
    vector<int> arr(size);
    for (int& x : arr) x = dis(gen);
    return arr;
}

// Измерение времени выполнения алгоритма сортировки
long long measureTime(void (*sortFunc)(vector<int>&), vector<int> arr) {
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto stop = high_resolution_clock::now();
    return duration_cast<milliseconds>(stop-start).count();
}

int main() {
    // Размеры тестируемых массивов
    vector<int> sizes = {1000, 5000, 10000};

    for (int size : sizes) {
        cout << "Массив размером: " << size << endl;
        vector<int> original = generateRandomArray(size); // Генерируем случайный массив

        // Копируем массивы отдельно для каждой сортировки
        vector<int> arrStandard = original;
        vector<int> arrOptimized = original;

        // Замер времени
        long long timeStandard = measureTime(bubbleSortStandard, arrStandard);
        long long timeOptimized = measureTime(bubbleSortOptimized, arrOptimized);

        // Вывод результатов
        cout << "Стандартная сортировка пузырьком: " << timeStandard << " мс" << endl;
        cout << "Оптимизированная сортировка пузырьком: " << timeOptimized << " мс" << endl;
        cout << "~~~~~~~~~~~~~ ★ ☆ ★ ☆ ★ ~~~~~~~~~~~~~" << endl;

    }

    return 0;
}
