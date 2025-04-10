#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class Entity {
protected:
    std::string name;
    int health;
    int attackPower;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attackPower(a), defense(d) {
    }

    virtual void attack(Entity& target) {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }

    virtual void heal(int amount) {
        health += amount;
        std::cout << name << " heals for " << amount << " HP.\n";
    }

    // Геттеры и сеттеры
    std::string getName() const { return name; }
    int getHealth() const { return health; }
    void setHealth(int h) { health = h; }
    void reduceHealth(int amount) { health -= amount; }
    int getAttack() const { return attackPower; }
    int getDefense() const { return defense; }

    virtual ~Entity() {}
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 20) {
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void heal(int amount) override {
        health += amount;
        std::cout << name << " uses a healing potion and recovers " << amount << " HP!\n";
    }

    void displayInfo() const override {
        std::cout << "Character: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 30) {
                damage += 5;
                std::cout << "Poisonous attack! ";
            }
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

class Boss : public Monster {
public:
    Boss(const std::string& n, int h, int a, int d)
        : Monster(n, h, a, d) {
    }

    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 40) {
                damage += 10;
                std::cout << "Flaming strike! ";
            }
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Boss: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Boss dragon("Dragon", 150, 30, 20);

    Entity* entities[] = { &hero, &goblin, &dragon };

    std::cout << "=== Characters ===\n";
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    std::cout << "\n=== Battle starts! ===\n";
    hero.attack(goblin);
    goblin.attack(hero);
    dragon.attack(hero);

    std::cout << "\n=== After battle ===\n";
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    std::cout << "\n=== Healing ===\n";
    hero.heal(20);

    std::cout << "\n=== Final Status ===\n";
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    return 0;
}
