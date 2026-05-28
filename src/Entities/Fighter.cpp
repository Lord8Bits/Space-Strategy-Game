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
        _agility(GameConstants::FIGHTER_AGILITY) {}

bool Fighter::tryDodge(const Ship& attacker) const{
    //A simple system to give a chance of dodging
    int roll = rand() % 100;

    int dodgeChance = _agility - attacker.getAttackPower() / 2;
    if (dodgeChance < 5) dodgeChance = 5;
    if (dodgeChance > 80) dodgeChance = 80;

    return dodgeChance > roll;
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