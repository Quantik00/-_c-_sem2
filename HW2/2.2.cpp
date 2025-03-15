#include <iostream>
#include <map>
#include <string>

using namespace std;

class PhoneBook {
private:
    map<string, string> contacts; // наш контейнер

public:
    //добавление
    void addContact(const string& name, const string& phone) { // не создаём лишние копии с помощью ссылки и данные сохраняем
        contacts[name] = phone;
        cout << "Добавлен контакт: " << name << " - " << phone << "\n";
    }

    //стирание
    void removeContact(const string& name) {
        if (contacts.erase(name)) {
            cout << "Контакт удален: " << name << "\n";
        } else {
            cout << "Контакт не найден: " << name << "\n";
        }
    }

    // поиск
    void searchContact(const string& name) const {
        auto it = contacts.find(name);
        if (it != contacts.end()) {
            cout << "Найден контакт: " << it->first << " - " << it->second << "\n";
        } else {
            cout << "Контакт не найден: " << name << "\n";
        }
    }

    // вывод всей инфы
    void displayContacts() const {
        if (contacts.empty()) {
            cout << "Телефонная книга пуста." << "\n";
            return;
        }
        cout << "Список контактов:" << "\n";
        for (const auto& [name, phone] : contacts) {
            cout << name << " - " << phone << "\n";
        }
    }
};

int main() {
    PhoneBook my_phoneBook;
    int choice;
    string name, phone;

    do {
        cout << "\nМеню:" << "\n";
        cout << "1. Добавить контакт" << "\n";
        cout << "2. Удалить контакт" << "\n";
        cout << "3. Найти контакт" << "\n";
        cout << "4. Вывести все контакты" << "\n";
        cout << "5. Выход" << "\n";
        cout << "Выберите: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Имя: ";
                cin >> name;
                cout << "Номер: ";
                cin >> phone;
                my_phoneBook.addContact(name, phone);
                break;
            case 2:
                cout << "Контакт для удаления: ";
                cin >> name;
                my_phoneBook.removeContact(name);
                break;
            case 3:
                cout << "Поиск: ";
                cin >> name;
                my_phoneBook.searchContact(name);
                break;
            case 4:
                my_phoneBook.displayContacts();
                break;
            case 5:
                cout << "Выход" << "\n";
                break;
            default:
                cout << "Что то не то!" << "\n";
        }
    } while (choice != 5);

    return 0;
}
