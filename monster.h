#ifndef DUNGEONS_AND_DRAGONS_SMART_MONSTER_H
#define DUNGEONS_AND_DRAGONS_SMART_MONSTER_H

#include <iostream>
#include <string>
#include <exception>

using namespace std;

class Monster {
public:
    Monster(string_view name_, unsigned health_, unsigned attack_) : name{name_}, health{health_}, attack{attack_} {
        if (name.empty())
            throw runtime_error{"Monster name must not be empty"};
        if (health == 0)
            throw runtime_error{"Monster health >= 1"};
        if (attack == 0)
            throw runtime_error{"Monster attack >= 1"};
    }

    virtual ~Monster() = default;

    [[nodiscard]] virtual unsigned calculate_damage(unsigned dmg) const noexcept = 0;

    void take_damage(unsigned dmg) noexcept {
        if (auto damage = calculate_damage(dmg); health > damage)
            health -= damage;
        else
            health = 0;
    }

    [[nodiscard]] bool is_dead() const noexcept {
        return health == 0;
    }

    [[nodiscard]] virtual string additional_information() const noexcept = 0;

    [[nodiscard]] unsigned get_attack() const noexcept {
        return attack;
    }

    [[nodiscard]] unsigned get_health() const noexcept {
        return health;
    }

    [[nodiscard]] const string &get_name() const noexcept {
        return name;
    }

    friend ostream &operator<<(ostream &o, const Monster &h) noexcept {
        o << '[' << h.name << ", " << h.health << " HP, " << h.attack << " ATK" << h.additional_information() << ']';
        return o;
    }

private:
    string name;
    unsigned health = 0;
    unsigned attack = 0;
};

class Elite_Monster : public Monster {
public:
    Elite_Monster(string_view name_, unsigned health_, unsigned attack_, unsigned defense_)
            : Monster(name_, health_, attack_), defense{defense_} {
        if (defense == 0)
            throw runtime_error{"Elite_Monster defense >= 1"};
    }

    [[nodiscard]] unsigned calculate_damage(unsigned dmg) const noexcept override {
        if (dmg > defense)
            return dmg - defense;
        else
            return 0;
    }

    [[nodiscard]] string additional_information() const noexcept override {
        return ", " + to_string(defense) + " DEF";
    }

private:
    unsigned defense = 0;
};

class Standard_Monster : public Monster {
public:
    Standard_Monster(string_view name_, unsigned health_, unsigned attack_) : Monster(name_, health_, attack_) {}

    [[nodiscard]] unsigned calculate_damage(unsigned dmg) const noexcept override {
        return dmg;
    }

    [[nodiscard]] string additional_information() const noexcept override {
        return "";
    }
};

#endif
