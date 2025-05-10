#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

//--------------------------------------------------------------------------------------
// Структура Book
//--------------------------------------------------------------------------------------
struct Book {
    int         id;
    std::string author;
    std::string title;
};

//--------------------------------------------------------------------------------------
// Класс Red-Black Tree для хранения книг по ключу id: вставка, удаление, поиск, вывод всех элементов
//--------------------------------------------------------------------------------------
class RBTree {
private:
    struct Node {
        Book   data;
        Node*  parent;
        Node*  left;    //левое дитя
        Node*  right;    //правое дитя
        bool   red;    // true если узел red
        explicit Node(const Book& b)
            : data(b), parent(nullptr), left(nullptr), right(nullptr), red(true) {}
    };

    Node* root;
    Node* NIL; // спец узел вместо nullptr, всегда черный (для упрощения логики)


// Балансируем дерево при вставке и удалении: реализуем повороты 
    // Левый поворот вокруг x
    void leftRotate(Node* x) {
        Node* y = x->right;
        //Делаем b правым потомком x
        x->right = y->left;
        if (y->left != NIL)
            //Переносим родителя x к y
            y->left->parent = x;
        // Обновляем ссылку родителя: Если x был корнем → y становится корнем иначе заменяем x на y у родителя
        y->parent = x->parent;
        if (x->parent == NIL)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        //Делаем x левым потомком y
        y->left = x;
        x->parent = y;
    }
// рассматриваем, что у узла х правый потомок у 
//Делаем b правым потомком x
// 




    // Правый поворот вокруг x
// Тут всё зеркально от левого
    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != NIL)
            y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == NIL)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    // Восстановление свойств после вставки z
// чтобы корень был чёрным и у красного не было красного
    void insertFixup(Node* z) {
        while (z->parent->red) {
            Node* grand = z->parent->parent;
            if (z->parent == grand->left) {
                Node* uncle = grand->right;
                if (uncle->red) { // дядя красный : Перекрашиваем родителя и дядю в черный, деда в красный, перемещаем z на деда для проверки выше
                    z->parent->red = false;
                    uncle->red     = false;
                    grand->red     = true;
                    z = grand;
                } else {
                    if (z == z->parent->right) { 
                        // дядя чёрный z- правый потомок : Поворачиваем родителя влево, теперь z стал левым потомком
                        z = z->parent;
                        leftRotate(z);
                    }
                         // дядя чёрный, z - левый потомок: Перекрашиваем родителя в черный, деда в красный ,поворачиваем деда вправо
                    z->parent->red = false;
                    grand->red     = true;
                    rightRotate(grand);
                }
            } else {
                                // Симметричные случаи для правого поддерева
                Node* uncle = grand->left;
                if (uncle->red) {
                    z->parent->red = false;
                    uncle->red     = false;
                    grand->red     = true;
                    z = grand;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->red = false;
                    grand->red     = true;
                    leftRotate(grand);
                }
            }
        }
        root->red = false; // по правилам корень всегда чёрный
    }

    // Трансплантация поддерева u на место v (замена одного поддерева другим)
    void transplant(Node* u, Node* v) {
        if (u->parent == NIL)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        v->parent = u->parent;
    }

    // Находит минимальный узел в поддереве x
    Node* minimum(Node* x) const {
        while (x->left != NIL)
            x = x->left;
        return x;
    }

    // Восстановление свойств после удаления x
    void deleteFixup(Node* x) {
        while (x != root && !x->red) { // нарушилась "высота чёрных"
            if (x == x->parent->left) {
                Node* sib = x->parent->right;
                if (sib->red) { 
                    // брат красный
                    // Перекрашиваем брата и родителя, поворачиваем родителя влево, переходим к другим случаям
                    sib->red        = false;
                    x->parent->red  = true;
                    leftRotate(x->parent);
                    sib = x->parent->right;
                }
                if (!sib->left->red && !sib->right->red) {
                    // оба потомка брата чёрные
                    // Перекрашиваем брата в красный, перемещаем x на родителя
                    sib->red = true;
                    x = x->parent;
                } else {
                    if (!sib->right->red) {
                        // правый потомок брата черный
                        // Перекрашиваем левого потомка брата в черный, поворачиваем брата вправо
                        sib->left->red = false;
                        sib->red       = true;
                        rightRotate(sib);
                        sib = x->parent->right;
                    }
                    // правый потомок брата красный
                    // Перекрашиваем брата в цвет родителя, родителя и правого потомка в черный, поворачиваем родителя влево
                    sib->red        = x->parent->red;
                    x->parent->red  = false;
                    sib->right->red = false;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                // симметрично для правого подддерева
                Node* sib = x->parent->left;
                if (sib->red) {
                    sib->red        = false;
                    x->parent->red  = true;
                    rightRotate(x->parent);
                    sib = x->parent->left;
                }
                if (!sib->right->red && !sib->left->red) {
                    sib->red = true;
                    x = x->parent;
                } else {
                    if (!sib->left->red) {
                        sib->right->red = false;
                        sib->red        = true;
                        leftRotate(sib);
                        sib = x->parent->left;
                    }
                    sib->red        = x->parent->red;
                    x->parent->red  = false;
                    sib->left->red  = false;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->red = false;
    }

    // Удаление узла z из дерева: Находим узел y для удаления: Если у узла меньше двух потомков → y = z, иначе y = преемник (минимальный в правом поддереве)
    //Заменяем y на его потомка x через transplant(), если y был черным → вызываем deleteFixup(x)
    void deleteNode(Node* z) {
        Node* y = z;
        Node* x;
        bool wasRed = y->red;

        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            wasRed = y->red;
            x = y->right;
            if (y->parent == z)
                x->parent = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left   = z->left;
            y->left->parent = y;
            y->red    = z->red;
        }
        delete z;
        if (!wasRed)
            deleteFixup(x);
    }

    // In-order обход для записи в файл (Рекурсивная запись данных в файл в порядке возрастания id)
    void inOrderWrite(Node* x, std::ofstream& out) const {
        if (x == NIL) return;
        inOrderWrite(x->left, out); // рекурсия влево
        out << x->data.id << ',' // пишем текущий  узел 
            << x->data.author << ','
            << x->data.title << '\n';
        inOrderWrite(x->right, out); // рекурсия вправо
    }

    // Рекурсивный поиск по id
    Node* searchNode(Node* x, int id) const {
        if (x == NIL || id == x->data.id)
            return x;
        return id < x->data.id
            ? searchNode(x->left, id)
            : searchNode(x->right, id);
    }

public:
   // Конструктор: инициализирует NIL и root
    RBTree() {
        NIL = new Node({0, "", ""});
        NIL->red = false;
        NIL->left = NIL->right = NIL->parent = NIL;
        root = NIL;
    }

    ~RBTree() {
        // TODO: рекурсивное удаление всех узлов (необходимо для чистки памяти)
    }

    // Вставка книги
    void insert(const Book& b) {
        Node* z = new Node(b);
        z->left = z->right = z->parent = NIL;

        Node* y = NIL;
        Node* x = root;
        while (x != NIL) {
            y = x;
            x = (z->data.id < x->data.id) ? x->left : x->right;
        }
        z->parent = y;
        if (y == NIL)        root = z;
        else if (z->data.id < y->data.id) y->left = z;
        else                y->right = z;

        insertFixup(z);
    }

    // Удаление книги по id
    void remove(int id) {
        Node* z = searchNode(root, id);
        if (z != NIL) deleteNode(z);
    }

    // Поиск книги по id; возвращает nullptr, если не найдено
    Book* search(int id) const {
        Node* res = searchNode(root, id);
        return (res == NIL) ? nullptr : &res->data;
    }

    // Запись всех книг в файл (сортировка по возрастанию id)
    void writeAll(const std::string& filename) const {
        std::ofstream out(filename);
        inOrderWrite(root, out);
    }
};

// Генерация случайной строки заданной длины
static std::string randomString(int length) {
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, sizeof(alphanum) - 2);

    std::string s;
    s.reserve(length);
    for (int i = 0; i < length; ++i) {
        s += alphanum[dist(gen)];
    }
    return s;
}

int main() {
    constexpr int N = 10000;
    std::vector<Book> books;
    books.reserve(N);

    // Генерация случайных книг
    for (int i = 1; i <= N; ++i) {
        books.push_back({i, randomString(10), randomString(20)});
    }
    std::shuffle(books.begin(), books.end(), std::mt19937{std::random_device{}()});

    using Clock = std::chrono::steady_clock;

    // 1) Вставка в RBTree
    RBTree tree;
    auto t0 = Clock::now();
    for (const auto& b : books) tree.insert(b);
    auto t1 = Clock::now();
    auto rbInsertUs = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

    // 2) Вставка в std::map
    std::map<int, Book> mp;
    t0 = Clock::now();
    for (const auto& b : books) mp[b.id] = b;
    t1 = Clock::now();
    auto mapInsertUs = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

    // Подготовка ключей для поиска
    std::vector<int> keys;
    keys.reserve(N);
    for (const auto& b : books) keys.push_back(b.id);
    std::shuffle(keys.begin(), keys.end(), std::mt19937{std::random_device{}()});

    // 3) Поиск в RBTree
    t0 = Clock::now();
    for (int id : keys) tree.search(id);
    t1 = Clock::now();
    auto rbSearchUs = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

    // 4) Поиск в std::map
    t0 = Clock::now();
    for (int id : keys) mp.find(id);
    t1 = Clock::now();
    auto mapSearchUs = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

    // Вывод результатов
    std::cout << "Вставка " << N << " книг:\n"
              << "  RBTree:    " << rbInsertUs  << " мкс\n"
              << "  std::map:  " << mapInsertUs << " мкс\n"
              << "Поиск "   << N << " книг:\n"
              << "  RBTree:    " << rbSearchUs  << " мкс\n"
              << "  std::map:  " << mapSearchUs << " мкс\n";

    // Запись в файлы (необязательно выводить время)
    tree.writeAll("rbt_sorted.txt");
    std::ofstream fout("map_sorted.txt");
    for (const auto& kv : mp) {
        fout << kv.second.id << ','
             << kv.second.author << ','
             << kv.second.title << '\n';
    }

    return 0;
}
