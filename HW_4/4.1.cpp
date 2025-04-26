#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>

using namespace std;
using namespace chrono;

// Пузырькек
// - Проходим по массиву, попарно сравнивая соседние элементы
// - Самые большие "всплывают" в конец массива на каждом проходе
// - Повторяем
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n -1; i++) {
        for (int j = 0; j < n - i -1; j++) {
            // Если текущий элемент больше следующего, меняем местами
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Быстрая сортировка (QuickSort)
// - Выбираем опорный элемент (пивот) и делим массив на меньше и больше
// - Рекурсивно сортируем обе части
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        // Опорный элемент — последний элемент подмассива
        int pivot = arr[high];
        int i = low -1;
        // Разделение массива по принципу: меньшие слева, большие справа
        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i+1], arr[high]);
        int pi = i+1; // Индекс, где стоит опорный элемент

        // Рекурсивно сортируем левую и правую части массива
        quickSort(arr, low, pi-1);
        quickSort(arr, pi+1, high);
    }
}

// Слияние двух отсортированных частей массива для сортировки слиянием
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid-left+1;
    int n2 = right-mid;
    vector<int> L(n1), R(n2);

    // Копируем данные во временные массивы L[] и R[]
    for (int i = 0; i < n1; i++) L[i] = arr[left+i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid+1+j];

    // Объединяем временные массивы обратно в arr[left..right]
    int i = 0, j = 0, k = left;
    while (i<n1 && j<n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    // Копируем оставшиеся элементы, если остались
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

// Сортировка слиянием (MergeSort)
// - Рекурсивно делим массив на две части до единичных элементов
// - Сливаем части обратно, упорядочивая элементы
void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left+(right-left)/2;
        // Рекурсивно сортируем две половины
        mergeSort(arr, left, mid);
        mergeSort(arr, mid+1, right);
        // Сливаем отсортированные половины
        merge(arr, left, mid, right);
    }
}

// Сортировка выбором (Selection Sort)
// - Находим минимальный элемент в неотсортированной части
// - Меняем его с первым элементом этой части
// - Постепенно формируем отсортированную часть слева
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i=0; i<n-1; i++) {
        int minIdx = i;
        // Находим минимальный элемент в неотсортированной части
        for (int j=i+1; j<n; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        // Меняем найденный минимальный элемент с первым элементом
        swap(arr[minIdx], arr[i]);
    }
}

// Генерация случайного массива заданного размера
vector<int> generateRandomArray(int size) {
    random_device rd; // Источник случайности
    mt19937 gen(rd()); // Генератор случайных чисел
    uniform_int_distribution<> dis(1, 100000); // Диапазон значений
    vector<int> arr(size);
    for (int& x : arr) x = dis(gen); // Заполняем массив случайными числами
    return arr;
}

// Измерение времени выполнения алгоритма сортировки
long long measureTime(void (*sortFunc)(vector<int>&), vector<int> arr) {
    auto start = high_resolution_clock::now(); // Начало отсчета времени
    sortFunc(arr); // Выполняем сортировку
    auto stop = high_resolution_clock::now(); // Конец отсчета времени
    return duration_cast<milliseconds>(stop-start).count(); // Возвращаем время в миллисекундах
}

// Замер времени для QuickSort
long long measureTimeQuickSort(vector<int> arr) {
    auto start = high_resolution_clock::now();
    quickSort(arr, 0, arr.size()-1);
    auto stop = high_resolution_clock::now();
    return duration_cast<milliseconds>(stop-start).count();
}

// Специальная функция замера времени для MergeSort
long long measureTimeMergeSort(vector<int> arr) {
    auto start = high_resolution_clock::now();
    mergeSort(arr, 0, arr.size()-1);
    auto stop = high_resolution_clock::now();
    return duration_cast<milliseconds>(stop-start).count();
}

int main() {
    // Размеры массивов для тестирования
    vector<int> sizes = {1000, 5000, 10000};

    for (int size : sizes) {
        cout << "Массив размером: " << size << endl;
        vector<int> original = generateRandomArray(size); // Генерируем массив

        vector<pair<string, long long>> results; // Сохраняем пары (название алгоритма, время)

        // Замеряем время всех сортировок
        results.push_back({"Быстрая сортировка", measureTimeQuickSort(original)});
        results.push_back({"Сортировка слиянием", measureTimeMergeSort(original)});
        results.push_back({"Сортировка пузырьком", measureTime(bubbleSort, original)});
        results.push_back({"Сортировка выбором", measureTime(selectionSort, original)});

        // Сортируем результаты по возрастанию времени (от самых быстрых к самым медленным)
        sort(results.begin(), results.end(), [](auto& a, auto& b) {
            return a.second < b.second;
        });

        // Выводим результаты
        for (auto& res : results) {
            cout << res.first << ": " << res.second << " мс" << endl;
        }
        cout << "^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^" << endl;
    }

    return 0;
}
