#include <iostream>
#include <string>

// Базовый класс
class Entity {
protected:
    std::string name;  // Защищенное поле: имя
    int health;        // Защищенное поле: здоровье

public:
    // Конструктор базового класса
    Entity(const std::string& n, int h) : name(n), health(h) {}

    // Виртуальный метод для вывода информации
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }

    virtual ~Entity() {}
};

// Класс Player, наследует от Entity
class Player : public Entity {
private:
    int experience; // Приватное поле: опыт

public:
    // Конструктор
    Player(const std::string& n, int h, int exp)
        : Entity(n, h), experience(exp) {
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        Entity::displayInfo(); // Вызов метода базового класса
        std::cout << "Experience: " << experience << std::endl;
    }
};

// Класс Enemy, наследует от Entity
class Enemy : public Entity {
private:
    std::string type; // Приватное поле: тип врага

public:
    // Конструктор
    Enemy(const std::string& n, int h, const std::string& t)
        : Entity(n, h), type(t) {
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        Entity::displayInfo(); // Вызов метода базового класса
        std::cout << "Type: " << type << std::endl;
    }
};

// Новый класс Boss, наследует от Enemy
class Boss : public Enemy {
private:
    std::string specialAbility; // Уникальное поле: специальная способность

public:
    // Конструктор
    Boss(const std::string& n, int h, const std::string& t, const std::string& ability)
        : Enemy(n, h, t), specialAbility(ability) {
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        Enemy::displayInfo(); // Вызов метода Enemy
        std::cout << "Special Ability: " << specialAbility << std::endl;
    }
};

int main() {
    // Создаем объекты игрока и врага
    Player hero("Hero", 100, 0);
    Enemy monster("Goblin", 50, "Goblin");

    // Выводим информацию о персонажах
    std::cout << "Player Info:" << std::endl;
    hero.displayInfo();

    std::cout << "\nEnemy Info:" << std::endl;
    monster.displayInfo();

    // Создаем объект босса и выводим его информацию
    Boss finalBoss("Dragon King", 200, "Dragon", "Firestorm");

    std::cout << "\nBoss Info:" << std::endl;
    finalBoss.displayInfo();

    return 0;
}