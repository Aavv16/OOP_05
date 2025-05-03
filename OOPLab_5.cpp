#include <iostream>
#include <queue>
#include <string>

// Шаблонный класс
template <typename T>
class Queue {
private:
    std::queue<T> data;

public:
    // Добавление в очередь
    void push(const T& item) {
        data.push(item);
        std::cout << "Pushed: " << item << std::endl;
    }

    // Удаление из очереди
    void pop() {
        if (!data.empty()) {
            std::cout << "Popped: " << data.front() << std::endl;
            data.pop();
        }
        else {
            std::cout << "Queue is empty, nothing to pop." << std::endl;
        }
    }

    // Просмотр первого элемента
    void front() const {
        if (!data.empty()) {
            std::cout << "Front: " << data.front() << std::endl;
        }
        else {
            std::cout << "Queue is empty." << std::endl;
        }
    }

    // Проверка, пустоты очереди
    bool isEmpty() const {
        return data.empty();
    }
};

// Проверка работы шаблона
int main() {
    std::cout << "--- line queue ---" << std::endl;
    Queue<std::string> stringQueue;
    stringQueue.push("Apple");
    stringQueue.push("Banana");
    stringQueue.front();
    stringQueue.pop();
    stringQueue.front();

    std::cout << "\n--- queue of numbers ---" << std::endl;
    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.front();
    intQueue.pop();
    intQueue.front();

    return 0;
}