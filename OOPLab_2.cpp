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
        std::cout << "Character " << name << " created!\n";
    }

    // Деструктор
    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Character - Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created!\n";
    }

    // Деструктор
    ~Monster() {
        std::cout << "Monster " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Monster - Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
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

    void displayInfo() const {
        std::cout << "Weapon - Name: " << name << ", Damage: " << damage
            << ", Weight: " << weight << "kg" << std::endl;
    }
};

int main() {
    std::cout << "     Game Start     \n";

    Character hero("Archer", 100, 20, 10);
    hero.displayInfo();

    Monster goblin("Goblin", 60, 15, 5);
    goblin.displayInfo();

    std::cout << "\n     Weapons     \n";

    Weapon sword("Steel Sword", 25, 3.5);
    Weapon bow("Long Bow", 18, 2.0);
    Weapon axe("Battle Axe", 35, 6.0);

    sword.displayInfo();
    bow.displayInfo();
    axe.displayInfo();

    std::cout << "\n     Game End     \n";
    return 0;
}