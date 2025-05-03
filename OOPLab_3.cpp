#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    // Перегрузка оператора == для сравнения по имени и здоровью
    bool operator==(const Character& other) const {
        return name == other.name && health == other.health;
    }

    // Перегрузка оператора << для вывода информации о персонаже
    friend std::ostream& operator<<(std::ostream& os, const Character& character) {
        os << "Character: " << character.name << ", HP: " << character.health
            << ", Attack: " << character.attack << ", Defense: " << character.defense;
        return os;
    }
};

class Weapon {
private:
    std::string name;
    int damage;
    float weight;

public:
    // Конструктор
    Weapon(const std::string& n, int dmg, float w)
        : name(n), damage(dmg), weight(w) {
        std::cout << "Weapon " << name << " created!\n";
    }

    // Деструктор
    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!\n";
    }

    // Перегрузка оператора + для увеличения урона оружия
    Weapon operator+(const Weapon& other) const {
        return Weapon(name + " & " + other.name, damage + other.damage, weight + other.weight);
    }

    // Перегрузка оператора > для сравнения оружий по урону
    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    // Перегрузка оператора << для вывода информации об оружии
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
        os << "Weapon: " << weapon.name << ", Damage: " << weapon.damage
            << ", Weight: " << weapon.weight << "kg";
        return os;
    }
};

int main() {
    // Создание персонажей
    Character hero1("Hero", 100, 20, 10);
    Character hero2("Hero", 100, 20, 10);
    Character hero3("Warrior", 150, 25, 15);

    // Сравнение персонажей
    if (hero1 == hero2) {
        std::cout << "Hero1 and Hero2 are the same!\n";
    }
    if (!(hero1 == hero3)) {
        std::cout << "Hero1 and Hero3 are different!\n";
    }

    // Вывод информации о персонаже
    std::cout << "\n" << hero1 << std::endl;
    std::cout << hero2 << std::endl;
    std::cout << hero3 << std::endl;

    // Создание оружия
    Weapon sword("Sword", 25, 3.5);
    Weapon bow("Bow", 18, 2.0);
    Weapon axe("Axe", 35, 6.0);

    // Перегрузка оператора + (создание нового оружия с увеличенным уроном)
    Weapon combinedWeapon = sword + bow;
    std::cout << "\nCombined Weapon: " << combinedWeapon << std::endl;

    // Перегрузка оператора > (сравнение оружий по урону)
    if (axe > sword) {
        std::cout << "Axe has more damage than Sword!\n";
    }
    else {
        std::cout << "Sword has more damage than Axe!\n";
    }

    return 0;
}