#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>

std::mutex monstersMutex;
std::mutex battleMutex;

bool battleInProgress = false;
bool heroAlive = true;

// ==== Классы ====

class Character {
public:
    std::string name;
    int health;
    int attack;
    int defense;

    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }

    void displayInfo() const {
        std::cout << name << " | HP: " << health << ", ATK: " << attack << ", DEF: " << defense << "\n";
    }

    bool isAlive() const {
        return health > 0;
    }
};

class Monster {
public:
    std::string type;
    int health;
    int attack;
    int defense;

    Monster(const std::string& t, int h, int a, int d)
        : type(t), health(h), attack(a), defense(d) {
    }

    void displayInfo() const {
        std::cout << "Monster: " << type << " | HP: " << health << ", ATK: " << attack << ", DEF: " << defense << "\n";
    }

    bool isAlive() const {
        return health > 0;
    }
};

// ==== Глобальные объекты ====
std::vector<Monster> monsters;
Character hero("Hero", 100, 20, 10);

// ==== Генерация монстров ====
void generateMonsters() {
    std::vector<std::string> types = { "Goblin", "Orc", "Troll", "Skeleton" };
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    while (heroAlive) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::lock_guard<std::mutex> lock(monstersMutex);
        std::string type = types[rand() % types.size()];
        monsters.emplace_back(type, 50 + rand() % 51, 10 + rand() % 11, 5 + rand() % 6);
        std::cout << "[+] New monster generated!\n";
    }
}

// ==== Бой между героем и монстром ====
void fight() {
    while (heroAlive) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(monstersMutex);

        if (!monsters.empty() && !battleInProgress) {
            battleInProgress = true;
            Monster& monster = monsters.front();

            std::cout << "\n⚔️ Battle started between Hero and " << monster.type << "!\n";

            while (monster.isAlive() && hero.isAlive()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));

                // Hero attacks
                int heroDamage = std::max(0, hero.attack - monster.defense);
                monster.health -= heroDamage;
                std::cout << "Hero hits " << monster.type << " for " << heroDamage << " damage!\n";

                if (!monster.isAlive()) {
                    std::cout << monster.type << " is defeated!\n";
                    break;
                }

                // Monster attacks
                int monsterDamage = std::max(0, monster.attack - hero.defense);
                hero.health -= monsterDamage;
                std::cout << monster.type << " hits Hero for " << monsterDamage << " damage!\n";

                if (!hero.isAlive()) {
                    std::cout << "💀 Hero has been defeated!\n";
                    heroAlive = false;
                    break;
                }
            }

            if (!monsters.empty()) monsters.erase(monsters.begin());
            battleInProgress = false;
        }
    }
}

// ==== Главная функция ====
int main() {
    std::thread monsterThread(generateMonsters);
    std::thread battleThread(fight);

    while (heroAlive) {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        {
            std::lock_guard<std::mutex> lock(monstersMutex);
            std::cout << "\n[INFO] Current Monsters:\n";
            for (const auto& monster : monsters) {
                monster.displayInfo();
            }
        }

        {
            std::lock_guard<std::mutex> lock(battleMutex);
            std::cout << "[INFO] Hero status:\n";
            hero.displayInfo();
        }
    }

    monsterThread.join();
    battleThread.join();

    std::cout << "\n=== Game Over ===\n";
    return 0;
}