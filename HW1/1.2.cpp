#include <iostream>
#include <vector>

using namespace std;

// вставляем элемент в вектор, по возрастанию
void vstavka(vector<int>& vec, int value) {
    vector<int>::iterator it = vec.begin(); // итератор указывает на первый элемент
    while (it != vec.end() && *it < value) {
        ++it;
    }
    vec.insert(it, value); // вставка
}

// разделяем вход на + и - числа, + сортируем
void SplitAndSort(const vector<int>& input, vector<int>& positive, vector<int>& negative) {
    for (int num : input) {
        if (num > 0) {
            vstavka(positive, num);
        } else if (num < 0) {
            vstavka(negative, num);
        }
    }
}

int main() {
    
    vector<int> data = {5, -5, 6, -6, 7, -0, 0, 8};
    vector<int> positive, negative;
    
    SplitAndSort(data, positive, negative);

    cout << "Положительные: ";
    for (int num : positive) {
        cout << num << " ";
    }
    
    cout << "\nОтрицательные: ";
    for (int num : negative) {
        cout << num << " ";
    }
    cout << '\n';
    
    return 0;
}
