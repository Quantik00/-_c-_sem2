#include <iostream>
#include <map> // Хранение количества появлений слов (пара ключ значение)
#include <string>
#include <sstream> // Разбиение строки на слова
#include <cctype> // Для работы с символами (isalpha, tolower)

using namespace std;

//Подчищаем слово  и приводим к нижнему регистру
string normalize_word(const string& word) {
    string cleaned_word;
    for (char ch : word) {
        if (isalpha(ch)) { // Оставляем только буквы
            cleaned_word += tolower(ch); // К нижнему регистру
        }
    }
    return cleaned_word; // очистили слово
}

int main() {
    map<string, int> frequency_dict; // Здесь частота появления слова
    string input_line, word;
    
    // Читаем построчно
    while (getline(cin, input_line)) {
        stringstream line_stream(input_line); // Бьём строку на слова
        
        while (line_stream >> word) {
            string processed_word = normalize_word(word); // привели в порядок слово
            if (!processed_word.empty()) {
                frequency_dict[processed_word]++; // Увеличили счётчик
            }
        }
    }
    
    string most_frequent_word; // здесь будет самое частое слово храниться
    int highest_count = 0; // здесь частота
    
    // Ищем слово с высшей частотой
    for (const auto& entry : frequency_dict) {
        if (entry.second > highest_count || (entry.second == highest_count && entry.first < most_frequent_word)) {
            most_frequent_word = entry.first;
            highest_count = entry.second;
        } // Случ 1 частота строго больше, тогда очев. Случ 2 первее в словаре, частота та же, выбираем его
    }
    
    cout << most_frequent_word << endl;
    return 0;
}
