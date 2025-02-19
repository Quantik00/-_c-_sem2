#include <iostream>
#include <vector>
#include <string>
using namespace std;


template<typename Container>
void Print(const Container& container, const std::string& deliter) {
    bool first = true; //проверка на первенство в контейнере 
    for (const auto& element : container) {
        if (!first) {
            cout << deliter; // выводим разделитель, если не первый элемент
        }
        cout << element;
        first = false; // убираем первенство
    }
    cout << "\n"; // завершили строку
}

int main() {
    vector<int> data = {1, 2, 3};
    Print(data, ", ");  

    vector<string> words = {"Cute", "puppies", "and", "cats"};
    Print(words, " < - > "); 

    return 0;
}