#ifndef DUNGEONS_AND_DRAGONS_SMART_HERO_H
#define DUNGEONS_AND_DRAGONS_SMART_HERO_H

#include <string>
#include <map>
#include <exception>
#include <iostream>
#include <algorithm>

#include "hero_info.h"
#include "monster.h"

using namespace std;

class Hero {
public:
    Hero(string_view name_, Hero_Class hero_class_, Hero_Species hero_species_, unsigned max_hp_,
         const map<string, unsigned> &abilities_)
            : name{name_}, hero_class{hero_class_}, hero_species{hero_species_}, max_hp{max_hp_}, current_hp{max_hp_},
              abilities{abilities_} {
        if (name.empty())
            throw runtime_error{"Hero name must not be empty"};
        if (max_hp == 0)
            throw runtime_error{"max_hp >= 1"};
        if (abilities.size() != 6)
            throw runtime_error{"Hero abilities must have 6 values"};
        {
            auto it = abilities.find("Strength");
            if (it == abilities.end())
                throw runtime_error{"no Strength"};
            if (unsigned ability = it->second; ability == 0 || ability >= 21)
                throw runtime_error{"1 <= Strength <= 20"};
        }
        {
            auto it = abilities.find("Dexterity");
            if (it == abilities.end())
                throw runtime_error{"no Dexterity"};
            if (unsigned ability = it->second; ability == 0 || ability >= 21)
                throw runtime_error{"1 <= Dexterity <= 20"};
        }
        {
            auto it = abilities.find("Constitution");
            if (it == abilities.end())
                throw runtime_error{"no Constitution"};
            if (unsigned ability = it->second; ability == 0 || ability >= 21)
                throw runtime_error{"1 <= Constitution <= 20"};
        }
        {
            auto it = abilities.find("Intelligence");
            if (it == abilities.end())
                throw runtime_error{"no Intelligence"};
            if (unsigned ability = it->second; ability == 0 || ability >= 21)
                throw runtime_error{"1 <= Intelligence <= 20"};
        }
        {
            auto it = abilities.find("Wisdom");
            if (it == abilities.end())
                throw runtime_error{"no Wisdom"};
            if (unsigned ability = it->second; ability == 0 || ability >= 21)
                throw runtime_error{"1 <= Wisdom <= 20"};
        }
        {
            auto it = abilities.find("Charisma");
            if (it == abilities.end())
                throw runtime_error{"no Charisma"};
            if (unsigned ability = it->second; ability == 0 || ability >= 21)
                throw runtime_error{"1 <= Charisma <= 20"};
        }
        id = next_id++;
    }

    unsigned level_up() noexcept {
        if (level <= 19)
            ++level;
        return level;
    }

    [[nodiscard]] bool fight(Monster &m) {
        if (current_hp == 0)
            return false;

        const unsigned max_ability_value
                = max_element(
                        abilities.begin(), abilities.end(),
                        [](const auto &lhs, const auto &rhs) -> bool {
                            const auto &[key_lhs, value_lhs] = lhs;
                            const auto &[key_rhs, value_rhs] = rhs;
                            return value_lhs < value_rhs;
                        })->second;

        const unsigned damage = level * max_ability_value;
        const unsigned attack = m.get_attack();

        while (current_hp >= 1) {
            m.take_damage(damage);
            if (m.is_dead())
                return true;
            if (current_hp > attack)
                current_hp -= attack;
            else
                current_hp = 0;
        }
        return false;
    }

    [[nodiscard]] unsigned get_id() const noexcept {
        return id;
    }

    [[nodiscard]] unsigned get_level() const noexcept {
        return level;
    }

    [[nodiscard]] unsigned get_current_hp() const noexcept {
        return current_hp;
    }

    [[nodiscard]] unsigned get_max_hp() const noexcept {
        return max_hp;
    }

    [[nodiscard]] Hero_Class get_hero_class() const noexcept {
        return hero_class;
    }

    [[nodiscard]] Hero_Species get_hero_species() const noexcept {
        return hero_species;
    }

    [[nodiscard]] const string &get_name() const noexcept {
        return name;
    }

    friend ostream &operator<<(ostream &o, const Hero &obj) noexcept {
        o << '[' << obj.id << ", " << obj.name
          << ", (" << obj.hero_class << ", " << obj.hero_species << ", " << obj.level << "), {"
          << obj.abilities.at("Charisma") << ", "
          << obj.abilities.at("Constitution") << ", "
          << obj.abilities.at("Dexterity") << ", "
          << obj.abilities.at("Intelligence") << ", "
          << obj.abilities.at("Strength") << ", "
          << obj.abilities.at("Wisdom")
          << "}, (" << obj.current_hp << '/' << obj.max_hp << ") HP]";
        return o;
    }

private:
    inline static unsigned next_id = 0;

    unsigned id = 0;
    string name;
    Hero_Class hero_class = Hero_Class::BARBARIAN;
    Hero_Species hero_species = Hero_Species::DRAGONBORN;
    unsigned level = 1;
    unsigned max_hp = 0;
    unsigned current_hp = 0;
    map<string, unsigned> abilities;
};

#endif
