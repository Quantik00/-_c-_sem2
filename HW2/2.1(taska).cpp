#include <iostream>
#include <deque>
#include <string>

using namespace std;

// Класс задачи
struct Task {
    string description;  // наш кейс
    bool is_high_priority;  // true - высокий, false - низкий

    // Конструктор создания задач
    Task(const string& desc, bool high_priority)
        : description(desc), is_high_priority(high_priority) {}
};

// Класс для очереди задач
class TaskQueue {
private:
    deque<Task> queue;  // Очередь задач
public:

    // Добавление
    void add_task(const string& description, bool is_high_priority) {
        Task task(description, is_high_priority);
        
        if (is_high_priority) {
            queue.push_front(task);  // Высокий приоритет — в начало
        } 
        else {
            queue.push_back(task);  // Низкий приоритет — в конец
        }
        cout << "Задача добавлена: " << description << " (";
        if (is_high_priority) {
            cout << "high_p";
        } else {
            cout << "low_p";
        }
        cout << " priority)" << endl;
    }
    

    // Выполнение 
    void do_task() {
        if (queue.empty()) {
            cout << "Нет дел" << endl;
            return;
        }

        Task task = queue.front();  // Берем задачу из начала 
        queue.pop_front();  // Удаляем задачу (выполнили уже)
        cout << "Выполнение : " << task.description << " (";
        if (task.is_high_priority) {
            cout << "high_p";
        } else {
            cout << "low_p";
        }
        cout << " priority)" << endl;
    }

    // Число задач в очереди
    void queque_amount() const {
        if (queue.empty()) {
            cout << "Нет задач в очереди" << endl;
        } else {
            cout << "Активных задач: " << queue.size() << endl;
        }
    }
};

int main() {
    TaskQueue taskQueue;

    // Задачи
    taskQueue.add_task("Проснуться", true);  // Высокий приоритет
    taskQueue.add_task("Поесть", false);  // Низкий приоритет

    // Количество 
    taskQueue.queque_amount();

    // Выполняем 
    taskQueue.do_task();
    taskQueue.do_task();


    // Количество после выполнения
    taskQueue.queque_amount();

    // Попытка выполнить задачу, когда очередь пуста
    taskQueue.do_task();

    return 0;
}