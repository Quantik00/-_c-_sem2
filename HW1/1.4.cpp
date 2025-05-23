#include <iostream>
#include <stack> // Для работы со стеком (адаптор), (хранение открывающих скобок) (LIFO принцип -- last in first out)
#include <string> // Для работы со строками 
#include <unordered_map> // Для хранения пар скобок в хеш-таблице

using namespace std;

// Функция проверки правильной aka гармоничной скобочной последовательности
bool bracketPar(const string& s) {
    stack<char> stk;  // Стек для открывающих скобок
    
    // Словарь соответствия закрывающих скобок открывающим
    unordered_map<char, char> matching_bracket = {
        {')', '('},  
        {']', '['},  
        {'}', '{'}   
    };

    // Проходим по символьно в строке
    for (char c : s) {
        //Проверка текущего символа на открывтие
        if (c == '(' || c == '[' || c == '{') {
            stk.push(c);  // Добавили в стек
        } 
        // Символ - закрывающая скобка
        else {
            // Есть ли соответствующая открывающая скобка в стеке
            if (stk.empty() || stk.top() != matching_bracket[c]) {
                return false;  // Если стек пуст => последовательность неверная (нет открывающей пары)
            }
            stk.pop();  // Удаляем соответствующую открывающую скобку из стека
        }
    }
    
    // Стек пуст - все скобки сбалансированы
    // Не пуст - остались непарные открывающие скобки
    return stk.empty(); // вернёт если стек пуст True
}

int main() {
    string input;  // Храним вход
    
    cout << "Последовательность скобок: ";
    cin >> input;  

    cout << (bracketPar(input) ? "Гармоничная" : "Негармоничная") 
         << " )}]последовательность\n";
    
    return 0;  
}
