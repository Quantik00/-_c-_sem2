#include <iostream>
#include <vector> 
#include <thread> 
#include <random> 
#include <mutex>
#include <ctime> 

using namespace std;

const size_t ARRAY_SIZE = 5000; // основной массив
const size_t NUM_THREADS = 4;   // количество потоков

vector<int> data(ARRAY_SIZE);
mutex cout_mutex; // Для безопасного вывода из потоков

// Функция для заполнения части массива
void fillArrayPart(size_t start, size_t end, unsigned int seed, size_t thread_id) {
    mt19937 rng(seed); // Генератор случайных чисел: Сид (seed) зависит от текущего времени и ID потока — это гарантирует уникальность последовательности в каждом потоке.
    uniform_int_distribution<int> dist(1, 100); // Диапазон значений
// под каждый поток выделили свою область памяти, чтобы не было гонок данных
    
    for (size_t i = start; i < end; ++i) { // Заполняем часть массива
        data[i] = dist(rng); //
    }

    // Сообщаем о завершении работы
    lock_guard<mutex> lock(cout_mutex); // Защита вывода
    cout << "Поток #" << thread_id 
         << " заполнил элементы с " << start 
         << " по " << (end - 1) << endl;
}

int main() {
    vector<thread> threads;
    size_t chunk_size = ARRAY_SIZE / NUM_THREADS; // Размер части массива для каждого потока

    for (size_t i = 0; i < NUM_THREADS; ++i) { // Создаем и запускаем потоки
        size_t start = i * chunk_size; 
        size_t end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : start + chunk_size;// Обработка последнего потока: получает остаток, если размер массива не делится нацело    
        unsigned int seed = static_cast<unsigned int>(time(0)) + static_cast<unsigned int>(i * 100); // Уникальный сид для каждого потока

        threads.emplace_back(fillArrayPart, start, end, seed, i); // Запуск потока
    }

    for (auto& t : threads) {
        t.join();
    }

    // Покажем первые 10 чисел
    cout << "\nПервые 10 элементов массива:\n";
    for (int i = 0; i < 10; ++i) {
        cout << data[i] << " ";
    }
    cout << endl;

    return 0;
}
