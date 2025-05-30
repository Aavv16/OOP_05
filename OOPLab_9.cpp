#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <thread>
#include <sstream>

// Шаблонный класс Logger
template <typename T = std::string>
class Logger {
private:
    std::ofstream logFile;

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        char buf[80];
        std::tm tm;
        localtime_s(&tm, &in_time_t);
        strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S]", &tm);
        return std::string(buf);
    }

public:
    Logger(const std::string& filename) {
        logFile.open(filename, std::ios::app);
        if (!logFile) {
            throw std::runtime_error("Cannot open log file");
        }
    }

    ~Logger() {
        if (logFile.is_open()) logFile.close();
    }

    void log(const T& message) {
        std::ostringstream oss;
        oss << getCurrentTime() << " " << message;
        std::cout << oss.str() << std::endl;
        logFile << oss.str() << std::endl;
        logFile.flush();
    }
};

// Инвентарь
class Inventory {
private:
    std::vector<std::string> items;

public:
    void addItem(const std::string& item) {
        items.push_back(item);
    }

    void removeItem(const std::string& item) {
        items.erase(std::remove(items.begin(), items.end(), item), items.end());
    }

    void showInventory() const {
        std::cout << "Inventory: ";
        if (items.empty()) {
            std::cout << "empty" << std::endl;
        }
        else {
            for (const auto& item : items) {
                std::cout << "[" << item << "] ";
            }
            std::cout << std::endl;
        }
    }
};

// Класс Персонажа
class Character {
protected:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int level;
    int experience;
    Logger<>& logger;
    Inventory inventory;

public:
    Character(const std::string& n, int h, int a, int d, Logger<>& logger)
        : name(n), health(h), maxHealth(h), attack(a), defense(d),
        level(1), experience(0), logger(logger) {
        logger.log("Character " + name + " created");
    }

    virtual ~Character() = default;

    void attackEnemy(Character& enemy) {
        if (!enemy.isAlive()) {
            throw std::logic_error("Cannot attack a dead enemy");
        }
        int damage = attack - enemy.getDefense();
        if (damage > 0) {
            enemy.takeDamage(damage);
            logger.log(name + " attacks " + enemy.getName() + " for " + std::to_string(damage) + " damage");

            if (!enemy.isAlive()) {
                logger.log(enemy.getName() + " has been killed");
                gainExperience(30);
            }
        }
        else {
            logger.log(name + " attacks " + enemy.getName() + ", but it's ineffective");
        }
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
        logger.log(name + " takes " + std::to_string(damage) + " damage");
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        logger.log(name + " heals " + std::to_string(amount) + " HP");
    }

    void gainExperience(int exp) {
        experience += exp;
        if (experience >= 100) {
            level++;
            experience -= 100;
            maxHealth += 20;
            attack += 5;
            defense += 3;
            health = maxHealth;
            logger.log(name + " leveled up to " + std::to_string(level));
        }
    }

    void displayInfo() const {
        std::cout << name << " [Level: " << level << ", HP: " << health << "/" << maxHealth
            << ", ATK: " << attack << ", DEF: " << defense
            << ", EXP: " << experience << "/100]" << std::endl;
        inventory.showInventory();
    }

    bool isAlive() const { return health > 0; }
    std::string getName() const { return name; }
    int getDefense() const { return defense; }

    void addItem(const std::string& item) {
        inventory.addItem(item);
        logger.log(name + " picks up item: " + item);
    }

    void saveGame(const std::string& filename) {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Failed to save game");
        out << name << " " << health << " " << maxHealth << " " << attack << " "
            << defense << " " << level << " " << experience << std::endl;
    }

    void loadGame(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) throw std::runtime_error("Failed to load game");
        in >> name >> health >> maxHealth >> attack >> defense >> level >> experience;
    }
};

// Класс Монстра
class Monster : public Character {
public:
    Monster(const std::string& n, int h, int a, int d, Logger<>& logger)
        : Character(n, h, a, d, logger) {}

    virtual void specialAttack(Character& target) = 0;
};

// Конкретные монстры
class Goblin : public Monster {
public:
    Goblin(Logger<>& logger) : Monster("Goblin", 30, 8, 2, logger) {}

    void specialAttack(Character& target) override {
        int damage = attack - target.getDefense() + 2;
        if (damage > 0) {
            target.takeDamage(damage);
            logger.log(name + " uses special attack: " + std::to_string(damage) + " damage");
        }
    }
};

class Skeleton : public Monster {
public:
    Skeleton(Logger<>& logger) : Monster("Skeleton", 40, 10, 5, logger) {}

    void specialAttack(Character& target) override {
        int damage = attack - target.getDefense() + 3;
        if (damage > 0) {
            target.takeDamage(damage);
            logger.log(name + " uses special attack: " + std::to_string(damage) + " damage");
        }
    }
};

class Dragon : public Monster {
public:
    Dragon(Logger<>& logger) : Monster("Dragon", 100, 20, 10, logger) {}

    void specialAttack(Character& target) override {
        int damage = attack - target.getDefense() + 5;
        if (damage > 0) {
            target.takeDamage(damage);
            logger.log(name + " breathes fire for " + std::to_string(damage) + " damage");
        }
    }
};

// Битва
void battle(Character& hero, Monster& monster) {
    while (hero.isAlive() && monster.isAlive()) {
        try {
            hero.attackEnemy(monster);
            if (!monster.isAlive()) break;
            monster.specialAttack(hero);
        }
        catch (const std::exception& e) {
            std::cerr << "Battle error: " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// Главная функция
int main() {
    try {
        Logger<> logger("game_log.txt");
        logger.log("=== Game Started ===");

        Character hero("Hero", 100, 15, 5, logger);
        hero.addItem("Health Potion");
        hero.addItem("Iron Sword");

        // Сохранение и загрузка
        hero.saveGame("save.txt");
        hero.loadGame("save.txt");

        std::vector<std::unique_ptr<Monster>> monsters;
        monsters.push_back(std::make_unique<Goblin>(logger));
        monsters.push_back(std::make_unique<Skeleton>(logger));
        monsters.push_back(std::make_unique<Dragon>(logger));

        for (auto& monster : monsters) {
            logger.log("\n--- New Battle ---");
            hero.displayInfo();
            monster->displayInfo();
            std::cout << std::endl;

            battle(hero, *monster);

            if (!hero.isAlive()) {
                logger.log("Hero has fallen!");
                break;
            }

            hero.heal(20);
        }

        if (hero.isAlive()) {
            logger.log("Hero defeated all monsters!");
        }

        logger.log("=== Game Ended ===");
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
    }

    return 0;
}
