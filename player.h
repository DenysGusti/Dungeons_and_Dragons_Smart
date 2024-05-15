#ifndef DUNGEONS_AND_DRAGONS_SMART_PLAYER_H
#define DUNGEONS_AND_DRAGONS_SMART_PLAYER_H

#include <map>
#include <string>
#include <utility>
#include <exception>
#include <iostream>

using namespace std;

class Player {
public:
    Player(string_view first_name_, string_view last_name_) : first_name{first_name_}, last_name{last_name_} {
        if (first_name.empty())
            throw runtime_error{"Player first_name must not be empty"};
        if (last_name.empty())
            throw runtime_error{"Player last_name must not be empty"};
    }

    unsigned create_hero(string_view name, Hero_Class hero_class, Hero_Species hero_species, unsigned max_hp,
                         const map<string, unsigned> &abilities) {
        auto hero = make_shared<Hero>(name, hero_class, hero_species, max_hp, abilities);
        unsigned id = hero->get_id();
        ids.emplace(id, std::move(hero));
        return id;
    }

    void create_campaign(string_view name, unsigned min_level, unsigned id) {
        campaign = make_unique<Campaign>(name, min_level);
        auto it = ids.find(id);
        if (it == ids.end())
            throw runtime_error{"no such hero in ids"};
        campaign->add_hero(it->second);
    }

    void join_friends_campaign(Player &friend_, unsigned id) {
        if (!friend_.campaign)
            throw runtime_error{"friend doesn't have campaign"};
        auto it = ids.find(id);
        if (it == ids.end())
            throw runtime_error{"no such hero in ids"};
        friend_.campaign->add_hero(it->second);
    }

    void transfer_campaign(Player &friend_) {
        if (!campaign)
            throw runtime_error{"Player doesn't have campaign"};
        friend_.campaign = std::move(campaign);
    }

    unsigned remove_dead() noexcept {
        auto old_size = ids.size();
        for (auto first = ids.cbegin(), last = ids.cend(); first != last;) {
            const auto &[key, value] = *first;
            if (value->get_current_hp() == 0)
                first = ids.erase(first);
            else
                ++first;
        }
        return old_size - ids.size();
    }

    ostream &print_campaign(ostream &o) const noexcept {
        if (campaign)
            o << *campaign;
        else
            o << "[]";
        return o;
    }

    friend ostream& operator<<(ostream& o, const Player& p) {
        o << '[' << p.first_name << ' ' << p.last_name << ", {";
        for (const auto &[key, value]: p.ids)
            if (value) {
                o << *value;
                if (&value != &p.ids.crbegin()->second)
                    o << ", ";
            }
        o << "}, ";
        p.print_campaign(o) << ']';
        return o;
    }

private:
    string first_name;
    string last_name;
    map<unsigned, shared_ptr<Hero> > ids;
    unique_ptr<Campaign> campaign;
};

#endif
