#ifndef DUNGEONS_AND_DRAGONS_SMART_HERO_H
#define DUNGEONS_AND_DRAGONS_SMART_HERO_H

#include <string>
#include <map>
#include <exception>

#include "hero_info.h"

using namespace std;

class Hero {
public:
    Hero(string_view name_, Hero_Class hero_class_, Hero_Species hero_species_, unsigned max_hp_,
         const map<string, unsigned> &abilities_)
            : name{name_}, hero_class{hero_class_}, hero_species{hero_species_}, max_hp{max_hp_},
              abilities{abilities_}, current_hp{max_hp_} {
        if (name.empty())
            throw runtime_error{"name must not be empty"};
        if (max_hp == 0)
            throw runtime_error{"max_hp = 0"};
        if (abilities.size() != 6)
            throw runtime_error{"abilities must have 6 values"};

        if (abilities.find("Strength") == abilities.end())
            throw runtime_error{"no Strength"};
        if (auto ability = abilities["Strength"]; ability == 0 || ability >= 21)
            throw runtime_error{"1 <= Strength <= 20"};

        if (abilities.find("Dexterity") == abilities.end())
            throw runtime_error{"no Dexterity"};
        if (auto ability = abilities["Dexterity"]; ability == 0 || ability >= 21)
            throw runtime_error{"1 <= Dexterity <= 20"};

        if (abilities.find("Constitution") == abilities.end())
            throw runtime_error{"no Constitution"};
        if (auto ability = abilities["Constitution"]; ability == 0 || ability >= 21)
            throw runtime_error{"1 <= Constitution <= 20"};

        if (abilities.find("Intelligence") == abilities.end())
            throw runtime_error{"no Intelligence"};
        if (auto ability = abilities["Intelligence"]; ability == 0 || ability >= 21)
            throw runtime_error{"1 <= Intelligence <= 20"};

        if (abilities.find("Wisdom") == abilities.end())
            throw runtime_error{"no Wisdom"};
        if (auto ability = abilities["Wisdom"]; ability == 0 || ability >= 21)
            throw runtime_error{"1 <= Wisdom <= 20"};

        if (abilities.find("Charisma") == abilities.end())
            throw runtime_error{"no Charisma"};
        if (auto ability = abilities["Charisma"]; ability == 0 || ability >= 21)
            throw runtime_error{"1 <= Charisma <= 20"};

        id = next_id++;
    }

    unsigned level_up() noexcept {
        level = min(level + 1, 20u);
        return level;
    }

    friend ostream &operator<<(ostream &o, const Hero &obj) {
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
