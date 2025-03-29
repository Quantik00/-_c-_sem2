#include <iostream>
#include <fstream> // рдля работы с файлами
#include <map>
#include <string>

using namespace std;
// помним что map -- асс контейнер
map<string, string> parent; // Хранит родителя
map<string, int> height;    // высота
map<string, bool> people;   // известные люди

// Рекурсия для вычисления высоты
int count_height(const string& person) { // определенная ссылка на человека идёт на вход
    if (height.find(person) != height.end()) { // Если personа нет, он возвращает указатель за последним элементом).
        return height[person]; // Если уже рассчитано в мапе высоты, просто возвращаем
    }
  
    if (parent.find(person) == parent.end()) {
        return height[person] = 0; // Если родителя нет, то это--корень
    }
    return height[person] = count_height(parent[person]) + 1; // вызываем рекурсию для родителя текущего человека
}


int main() {
    ifstream input("input.txt"); // Открываем файл для чтения
    int N; // число элементов в дереве, строк будет N-1 шт
    input >> N;

    for (int i = 0; i < N - 1; ++i) {
        string child, par;
        input >> child >> par;
        parent[child] = par; // Записываем родителя
        people[child] = true;
        people[par] = true;
    }
    input.close();

    // Вычисляем высоты
    for (const auto& [person, _] : people) { // константная ссылка на ключи нашего мапа
        count_height(person);
    }

    // Выводим всех в лексикографическом порядке (работает благодаря тому что people это map)
    for (const auto& [person, _] : people) {
        cout << person << " " << height[person] << endl;
    }

    return 0;
}
