#ifndef DUNGEONS_AND_DRAGONS_SMART_HERO_INFO_H
#define DUNGEONS_AND_DRAGONS_SMART_HERO_INFO_H

#include <vector>
#include <string>

using namespace std;

enum class Hero_Class {
    BARBARIAN, BARD, CLERIC, DRUID, FIGHTER, MONK, PALADIN, RANGER, ROGUE, SORCERER, WARLOCK, WIZARD
};

enum class Hero_Species {
    DRAGONBORN, DWARF, ELF, GNOME, HALF_ELF, HALFLING, HALF_ORC, HUMAN, TIEFLING
};

const vector<string> hero_class_names{
        "Barbarian", "Bard", "Cleric", "Druid", "Fighter", "Monk", "Paladin", "Ranger", "Rogue", "Sorcerer", "Warlock",
        "Wizard"
};

const vector<string> hero_species_names{
        "Dragonborn", "Dwarf", "Elf", "Gnome", "Half-Elf", "Halfling", "Half-Orc",
        "Human", "Tiefling"
};

ostream &operator<<(ostream &o, Hero_Class m) {
    o << hero_class_names[static_cast<size_t>(m)];
    return o;
}

ostream &operator<<(ostream &o, Hero_Species m) {
    o << hero_species_names[static_cast<size_t>(m)];
    return o;
}

#endif
