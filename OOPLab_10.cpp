#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <sstream>
#include <functional>

using namespace std;

// -------------------- ENUM --------------------

enum class AccessLevel {
    Student = 1,
    Teacher = 2,
    Administrator = 3
};

string toString(AccessLevel level) {
    switch (level) {
    case AccessLevel::Student: return "Student";
    case AccessLevel::Teacher: return "Teacher";
    case AccessLevel::Administrator: return "Administrator";
    default: return "Unknown";
    }
}

AccessLevel fromInt(int val) {
    if (val < 1 || val > 3) throw invalid_argument("Invalid access level int");
    return static_cast<AccessLevel>(val);
}

// -------------------- Exceptions --------------------

class InvalidAccessLevelException : public exception {
public:
    const char* what() const noexcept override {
        return "Invalid access level!";
    }
};

class EmptyNameException : public exception {
public:
    const char* what() const noexcept override {
        return "Name cannot be empty!";
    }
};

// -------------------- Validation --------------------

namespace Validation {
    inline void checkName(const string& name) {
        if (name.empty()) throw EmptyNameException();
    }

    inline void checkAccessLevel(AccessLevel level) {
        if (level < AccessLevel::Student || level > AccessLevel::Administrator)
            throw InvalidAccessLevelException();
    }
}

// -------------------- User Base --------------------

class User {
protected:
    string name;
    int id;
    AccessLevel accessLevel;

public:
    User(const string& name, int id, AccessLevel level)
        : name(name), id(id), accessLevel(level) {
        Validation::checkName(name);
        Validation::checkAccessLevel(level);
    }

    virtual ~User() = default;

    string getName() const { return name; }
    int getId() const { return id; }
    AccessLevel getAccessLevel() const { return accessLevel; }

    void setName(const string& newName) {
        Validation::checkName(newName);
        name = newName;
    }

    void setId(int newId) { id = newId; }
    void setAccessLevel(AccessLevel level) {
        Validation::checkAccessLevel(level);
        accessLevel = level;
    }

    virtual void displayInfo() const {
        cout << "ID: " << id << ", Name: " << name
            << ", Access Level: " << static_cast<int>(accessLevel);
    }

    virtual string getType() const = 0;
    virtual string serialize() const = 0;

    static unique_ptr<User> deserialize(const string& line);
};

// -------------------- Derived Users --------------------

class Student : public User {
    string group;
public:
    Student(const string& name, int id, const string& group)
        : User(name, id, AccessLevel::Student), group(group) {}

    void displayInfo() const override {
        User::displayInfo();
        cout << ", Type: Student, Group: " << group << endl;
    }

    string getType() const override { return "Student"; }

    string serialize() const override {
        return "Student," + name + "," + to_string(id) + "," + to_string(static_cast<int>(accessLevel)) + "," + group;
    }
};

class Teacher : public User {
    string department;
public:
    Teacher(const string& name, int id, const string& department)
        : User(name, id, AccessLevel::Teacher), department(department) {}

    void displayInfo() const override {
        User::displayInfo();
        cout << ", Type: Teacher, Department: " << department << endl;
    }

    string getType() const override { return "Teacher"; }

    string serialize() const override {
        return "Teacher," + name + "," + to_string(id) + "," + to_string(static_cast<int>(accessLevel)) + "," + department;
    }
};

class Administrator : public User {
    string position;
public:
    Administrator(const string& name, int id, const string& position)
        : User(name, id, AccessLevel::Administrator), position(position) {}

    void displayInfo() const override {
        User::displayInfo();
        cout << ", Type: Administrator, Position: " << position << endl;
    }

    string getType() const override { return "Administrator"; }

    string serialize() const override {
        return "Administrator," + name + "," + to_string(id) + "," + to_string(static_cast<int>(accessLevel)) + "," + position;
    }
};

// -------------------- Resource --------------------

class Resource {
    string name;
    AccessLevel requiredAccess;
public:
    Resource(const string& name, AccessLevel level)
        : name(name), requiredAccess(level) {
        Validation::checkAccessLevel(level);
    }

    string getName() const { return name; }
    AccessLevel getRequiredAccessLevel() const { return requiredAccess; }

    bool checkAccess(const User& user) const {
        return static_cast<int>(user.getAccessLevel()) >= static_cast<int>(requiredAccess);
    }

    void displayInfo() const {
        cout << "Resource: " << name << ", Required Level: " << static_cast<int>(requiredAccess) << endl;
    }

    string serialize() const {
        return name + "," + to_string(static_cast<int>(requiredAccess));
    }

    static Resource deserialize(const string& line) {
        auto pos = line.find(',');
        if (pos == string::npos) throw invalid_argument("Invalid resource format");
        string name = line.substr(0, pos);
        int level = stoi(line.substr(pos + 1));
        return Resource(name, fromInt(level));
    }
};

// -------------------- User Deserialization --------------------

unique_ptr<User> User::deserialize(const string& line) {
    stringstream ss(line);
    vector<string> tokens;
    string token;
    while (getline(ss, token, ',')) tokens.push_back(token);

    if (tokens.size() < 5) throw invalid_argument("Invalid user format");

    string type = tokens[0];
    string name = tokens[1];
    int id = stoi(tokens[2]);
    AccessLevel level = fromInt(stoi(tokens[3]));
    string extra = tokens[4];

    if (type == "Student") return make_unique<Student>(name, id, extra);
    if (type == "Teacher") return make_unique<Teacher>(name, id, extra);
    if (type == "Administrator") return make_unique<Administrator>(name, id, extra);

    throw invalid_argument("Unknown user type");
}

// -------------------- Access Control System --------------------

template<typename T>
class AccessControlSystem {
    vector<unique_ptr<User>> users;
    vector<T> resources;

public:
    void addUser(unique_ptr<User> user) {
        users.push_back(move(user));
    }

    void addResource(const T& resource) {
        resources.push_back(resource);
    }

    bool checkAccess(int userId, const string& resName) const {
        auto user = find_if(users.begin(), users.end(),
            [userId](const unique_ptr<User>& u) { return u->getId() == userId; });
        auto res = find_if(resources.begin(), resources.end(),
            [resName](const T& r) { return r.getName() == resName; });
        return user != users.end() && res != resources.end() && res->checkAccess(**user);
    }

    void displayAllUsers() const {
        for (const auto& u : users) u->displayInfo();
    }

    void displayAllResources() const {
        for (const auto& r : resources) r.displayInfo();
    }

    User* findUserByName(const string& name) const {
        auto it = find_if(users.begin(), users.end(),
            [&name](const unique_ptr<User>& u) { return u->getName() == name; });
        return it != users.end() ? it->get() : nullptr;
    }

    void sortUsersBy(function<bool(const unique_ptr<User>&, const unique_ptr<User>&)> comp) {
        sort(users.begin(), users.end(), comp);
    }

    void saveToFile(const string& filename) const {
        ofstream out(filename);
        if (!out) throw runtime_error("Can't open file!");

        for (const auto& u : users) out << u->serialize() << "\n";

        out << "RESOURCES\n";
        for (const auto& r : resources) out << r.serialize() << "\n";
    }

    void loadFromFile(const string& filename) {
        ifstream in(filename);
        if (!in) throw runtime_error("Can't open file!");

        users.clear();
        resources.clear();

        string line;
        bool readingResources = false;
        while (getline(in, line)) {
            if (line.empty()) continue;
            if (line == "RESOURCES") {
                readingResources = true;
                continue;
            }

            if (!readingResources)
                users.push_back(User::deserialize(line));
            else
                resources.push_back(T::deserialize(line));
        }
    }
};

// -------------------- Main --------------------

int main() {
    try {
        AccessControlSystem<Resource> system;

        system.addUser(make_unique<Student>("Ivan Petrov", 1, "CS-101"));
        system.addUser(make_unique<Teacher>("Anna Volkova", 2, "Mathematics"));
        system.addUser(make_unique<Administrator>("Dr. Brown", 3, "Dean"));

        system.addResource(Resource("Library", AccessLevel::Student));
        system.addResource(Resource("Lab", AccessLevel::Teacher));
        system.addResource(Resource("Server Room", AccessLevel::Administrator));

        cout << "\n--- Users ---\n";
        system.displayAllUsers();

        cout << "\n--- Resources ---\n";
        system.displayAllResources();

        cout << "\n--- Access Checks ---\n";
        cout << "Ivan to Server Room: "
            << (system.checkAccess(1, "Server Room") ? "Granted" : "Denied") << endl;

        cout << "\n--- Sorted Users ---\n";
        system.sortUsersBy([](const auto& a, const auto& b) {
            return static_cast<int>(a->getAccessLevel()) < static_cast<int>(b->getAccessLevel());
            });
        system.displayAllUsers();

        system.saveToFile("data.txt");

        AccessControlSystem<Resource> loadedSystem;
        loadedSystem.loadFromFile("data.txt");

        cout << "\n--- Loaded from file ---\n";
        loadedSystem.displayAllUsers();
        loadedSystem.displayAllResources();

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
