#include "../../include/Technology.hpp"
#include <sstream>
#include <algorithm>

Technology::Technology(int id, const std::string& name, TechType type,
                       int max_level, Resource base_cost)
    : _id(id)
    , _name(name)
    , _type(type)
    , _level(0)           // always starts locked
    , _max_level(max_level)
    , _base_cost(base_cost)
{}

Resource Technology::getUpgradeCost() const {
    // Cost scales linearly: base * (level + 1)
    // operator* not defined on Resource, so scale each field manually
    const int multiplier = _level + 1;
    return {
        _base_cost.getGold()     * multiplier,
        _base_cost.getTitanium() * multiplier,
        _base_cost.getCadmium()  * multiplier
    };
}

void Technology::upgrade() {
    if (canUpgrade())
        _level++;
}

int Technology::getProductionBonus() const {
    if (_type != TechType::PRODUCTION) return 0;
    return _level * 10;
}

int Technology::getDefenseBonus() const {
    if (_type != TechType::DEFENSE) return 0;
    return _level * 5;
}

int Technology::getAttackBonus() const {
    if (_type != TechType::ATTACK) return 0;
    return _level * 8;
}

std::string Technology::getDetailedInfo() const {
    std::stringstream ss;
    ss << "=== " << _name << " ===\n";
    ss << "ID    : " << _id << "\n";
    ss << "Type  : ";
    switch (_type) {
        case TechType::PRODUCTION : ss << "Production"; break;
        case TechType::DEFENSE    : ss << "Defense";    break;
        case TechType::ATTACK     : ss << "Attack";     break;
    }
    ss << "\n";
    ss << "Level : " << _level << " / " << _max_level << "\n";
    ss << "Status: " << (isUnlocked() ? (isMaxed() ? "Maxed" : "Unlocked") : "Locked") << "\n";

    const Resource cost = getUpgradeCost();
    if (canUpgrade()) {
        ss << "Upgrade cost:\n";
        ss << "  Gold     : " << cost.getGold()     << "\n";
        ss << "  Titanium : " << cost.getTitanium() << "\n";
        ss << "  Cadmium  : " << cost.getCadmium()  << "\n";
    }

    if (_level > 0) {
        ss << "Bonuses:\n";
        if (_type == TechType::PRODUCTION) ss << "  Production : +" << getProductionBonus() << "\n";
        if (_type == TechType::DEFENSE)    ss << "  Defense    : +" << getDefenseBonus()    << "\n";
        if (_type == TechType::ATTACK)     ss << "  Attack     : +" << getAttackBonus()     << "\n";
    }

    return ss.str();
}