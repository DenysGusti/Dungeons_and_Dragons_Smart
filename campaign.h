#ifndef DUNGEONS_AND_DRAGONS_SMART_CAMPAIGN_H
#define DUNGEONS_AND_DRAGONS_SMART_CAMPAIGN_H

#include <map>
#include <string>
#include <memory>
#include <exception>
#include <iostream>

#include "monster.h"

using namespace std;

class Campaign {
public:
    Campaign(string_view name_, unsigned min_level_) : name{name_}, min_level{min_level_} {
        if (name.empty())
            throw runtime_error{"Campaign name must not be empty"};
        if (min_level == 0)
            throw runtime_error{"Campaign min_level >= 1"};
    }

    void add_hero(const shared_ptr<Hero> &h) {
        if (ids.find(h->get_id()) != ids.end())
            throw runtime_error{"hero is already in the campaign"};
        if (h->get_level() < min_level)
            throw runtime_error{"level of the hero is too small"};
        ids[h->get_id()] = h;
    }

    unsigned remove_dead_and_expired() noexcept {
        auto old_size = ids.size();
        for (auto first = ids.cbegin(), last = ids.cend(); first != last;) {
            const auto &[key, value] = *first;
            if (shared_ptr<Hero> hero = value.lock(); !hero || hero->get_current_hp() == 0)
                first = ids.erase(first);
            else
                ++first;
        }
        return old_size - ids.size();
    }

    void encounter_monster(unsigned id, Monster &m) {
        auto it = ids.find(id);
        if (it == ids.end())
            return;
        shared_ptr<Hero> hero = it->second.lock();
        if (!hero)
            return;
        bool won = hero->fight(m);
        if (won)
            hero->level_up();
    }

    friend ostream &operator<<(ostream &o, const Campaign &p) noexcept {
        o << '[' << p.name << " Campaign, Min_Level " << p.min_level << ", {";
        for (const auto &[key, value]: p.ids)
            if (auto hero = value.lock(); hero) {
                o << *hero;
                if (&value != &p.ids.crbegin()->second)
                    o << ", ";
            }
        o << "}]";
        return o;
    }

private:
    string name;
    map<unsigned, weak_ptr<Hero> > ids;
    unsigned min_level = 0;
};

#endif
