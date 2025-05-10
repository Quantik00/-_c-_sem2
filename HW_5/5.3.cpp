#include <iostream>  // Для ввода/вывода
#include <vector>    // Для использования std::vector
#include <thread>    // Для работы с потоками
#include <mutex>     // Для синхронизации потоков (мьютекс)
#include <algorithm> // Для алгоритмов сортировки
#include <future>    // Для работы с асинхронными результатами (promise/future)
#include <atomic>    // Для атомарных операций

std::mutex output_mutex;  // Мьютекс для синхронизированного вывода в консоль

// Функция для сортировки части массива: data - ссылка на массив, start - начальный индекс сортируемой части, end - конечный индекс (не включается)
void sort_chunk(std::vector<int>& data, size_t start, size_t end) {
    std::sort(data.begin() + start, data.begin() + end); // Сортировка части: получает диапазон индексов и сортирует этот кусок массива data через std::sort
}

// Функция для поиска значения в части массива: data - константная ссылка на массив, target - искомое значение, start - начальный индекс поиска, end - конечный индекс поиска, result - promise для возврата результата, found_flag - атомарный флаг для остановки других потоков при нахождении
void search_chunk(const std::vector<int>& data, int target, size_t start, size_t end, 
                 std::promise<size_t>&& result, std::atomic<bool>& found_flag) { // Параллельно ищет target в подмассиве data[start:end].
    //Нашла-> Ставит found_flag = true , Устанавливает result через promise.
    //не находит — передаёт -1.
    for (size_t i = start; i < end && !found_flag.load(); ++i) { // Поиск пока не найден или не дошли до конца 
        if (data[i] == target) {
            found_flag = true; // Устанавливаем флаг
            result.set_value(i);  // Возвращаем индекс
            return;
        }
    }
    result.set_value(-1); // Возвращаем -1 если не нашли
}

int main() {
    const size_t num_threads = 4;  // Количество потоков
    std::vector<int> data = {12, 3, 5, 7, 19, 8, 2, 4, 6, 1, 10, 11}; // Исходные данные
    size_t chunk_size = data.size() / num_threads; // Размер части для каждого потока

    std::vector<std::thread> sort_threads; // Вектор для хранения потоков сортировки

    // Параллельная сортировка частей массива : запускаем 4 потока
    for (size_t i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size; // Начало текущей части
        size_t end = (i == num_threads - 1) ? data.size() : start + chunk_size; // Конец (для последнего потока - до конца массива)
        sort_threads.emplace_back(sort_chunk, std::ref(data), start, end); // Создаем поток для сортировки части
    }

    // Ожидаем завершения всех потоков сортировки
    for (auto& t : sort_threads) {
        t.join(); // ждём завершения всех
    }

    // Объединение отсортированных частей
    while (chunk_size < data.size()) {
        for (size_t i = 0; i + chunk_size < data.size(); i += 2 * chunk_size) {
            size_t mid = i + chunk_size; // Середина для слияния
            size_t end = std::min(i + 2 * chunk_size, data.size()); // Конец слияния
            std::inplace_merge(data.begin() + i, data.begin() + mid, data.begin() + end); // Слияние двух частей с помощью merge
        }
        chunk_size *= 2; // Увеличиваем размер чанка в 2 раза
    }

    // Выводим отсортированный массив (с синхронизацией)
    {
        std::lock_guard<std::mutex> lock(output_mutex); // Блокируем мьютекс
        std::cout << "Отсортированный массив:\n";
        for (int num : data) std::cout << num << " ";
        std::cout << "\n";
    }

    // Параллельный поиск значения в массиве
    int target = 7; // Искомое значение
    std::atomic<bool> found_flag(false); // останавливаем потоки, если один уже нашел элемент
    std::vector<std::future<size_t>> futures; // Вектор для future результатов от каждого потока

    // поисковые потоки
    for (size_t i = 0; i < num_threads; ++i) {
        size_t start = i * (data.size() / num_threads); // Начало области поиска
        size_t end = (i == num_threads - 1) ? data.size() : start + (data.size() / num_threads); // Конец области, последний поток захватывает остаток

        std::promise<size_t> promise; // Promise для отправки результата из потока
        futures.push_back(promise.get_future()); // Сохраняем future
        // Создаем и сзапускаем поток
        std::thread(search_chunk, std::cref(data), target, start, end, 
                   std::move(promise), std::ref(found_flag)).detach();
    } // Получаем свою часть data, уже знаем, что ищем, передаёт promisе для возврата результата, следим за found_flag . (detach() означает, что поток работает независимо)

    // Обработка результатов поиска
    for (auto& fut : futures) { // проходимся по всем futuram (учитываем, что не копировать -- опасно для здровья)
        size_t result = fut.get(); // Получаем результат
        if (result != static_cast<size_t>(-1)) { // Если нашли
            std::lock_guard<std::mutex> lock(output_mutex); // Синхронизируем вывод
            std::cout << "Значение " << target << " найдено на позиции: " << result << "\n";
            return 0; 
        }
    }

    // Если не нашли
    std::cout << "Значение " << target << " не найдено.\n";
    return 0;
}
