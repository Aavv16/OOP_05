#include <iostream>
#include <queue>
#include <string>
#include <stdexcept>  

// Шаблонный класс
template <typename T>
class Queue {
private:
    std::queue<T> data;

public:
    // Добавление элемента
    void push(const T& item) {
        data.push(item);
        std::cout << "Pushed: " << item << std::endl;
    }

    // Удаление элемента с обработкой исключения
    void pop() {
        if (data.empty()) {
            throw std::runtime_error("Attempt to pop from an empty queue.");
        }
        std::cout << "Popped: " << data.front() << std::endl;
        data.pop();
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

// Проверка работы исключений
int main() {
    Queue<int> intQueue;

    try {
        std::cout << "--- Number Queue Test ---" << std::endl;
        intQueue.push(42);
        intQueue.pop();
        intQueue.pop();  // Здесь исключение
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    std::cout << "\n--- Line Queue Test ---" << std::endl;
    Queue<std::string> stringQueue;
    stringQueue.push("Hello");

    try {
        stringQueue.pop();
        stringQueue.pop();  // Здесь исключение
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}
