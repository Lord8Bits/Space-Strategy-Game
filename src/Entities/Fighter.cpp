#include <sstream>
#include <cstdlib>
#include "../../include/Fighter.hpp"
#include "../../include/CombatSystem.hpp"

Fighter::Fighter(const std::string& Name, const Vec2& Pos, Civilization* Owner) :
    Ship(Name, Pos, Owner,
        GameConstants::FIGHTER_MAX_HEALTH,
        GameConstants::FIGHTER_MOVEMENT_RANGE,
        GameConstants::FIGHTER_VISION_RANGE,
        GameConstants::FIGHTER_ATTACK,
        ShipType::FIGHTER),
        _agility(GameConstants::FIGHTER_AGILITY)
{
    _attackRange = GameConstants::FIGHTER_ATTACK_RANGE;
}

bool Fighter::tryDodge(const Ship& attacker) const {
    const int roll        = rand() % 100;
    int dodgeChance = _agility - attacker.getAttackPower() / 2;
    dodgeChance = std::max(5, std::min(80, dodgeChance));
    return dodgeChance > roll;
}

int Fighter::calculateDamageAgainst(const Entity& /*target*/) const {
    int bonus = _owner ? _owner->getAttackBonus() : 0;
    return _attackPower + bonus;
}

void Fighter::levelUp(){
    _level++;
    _health = _maxHealth += 10;
    _attackPower += 5;
    _agility += 3;
    if (_agility >= 70) _agility = 70;
    if (_level % 5 == 0){
        _movementRange++;
        _visionRange++;
    }
}

std::string Fighter::getDetailedInfo() const{
    std::stringstream ss;

    ss << Ship::getDetailedInfo() << std::endl;
    ss << "Agility: " << _agility << std::endl;

    return ss.str();
}