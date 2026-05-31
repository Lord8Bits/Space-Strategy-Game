#include <sstream>
#include <algorithm>
#include "Entities/Cruiser.hpp"

Cruiser::Cruiser(const std::string& Name, const Vec2& Pos, Civilization* Owner) :
    Ship(Name, Pos, Owner,
        GameConstants::CRUISER_MAX_HEALTH,
        GameConstants::CRUISER_MOVEMENT_RANGE,
        GameConstants::CRUISER_VISION_RANGE,
        GameConstants::CRUISER_ATTACK,
        ShipType::CRUISER),
        _shieldStrength(GameConstants::CRUISER_SHIELD)
{
    _attackRange = GameConstants::CRUISER_ATTACK_RANGE;
}

int Cruiser::absorbDamage(int damage) {
    if (damage <= 0) return 0;
    if (_shieldStrength <= 0) return damage;

    const int absorbed = std::min(_shieldStrength, damage);
    _shieldStrength -= absorbed;
    return damage - absorbed;
}

void Cruiser::levelUp(){
    _level++;
    _health = _maxHealth += 10;
    _attackPower += 10;
    _shieldStrength += 5;
    if (_shieldStrength >= 100) _shieldStrength = 100;
    if (_level % 5 == 0){
        _movementRange++;
        _visionRange++;
    }
}

std::string Cruiser::getDetailedInfo() const{
    std::stringstream ss;

    ss << Ship::getDetailedInfo() << std::endl;
    ss << "Shield: " << _shieldStrength << std::endl;

    return ss.str();
}