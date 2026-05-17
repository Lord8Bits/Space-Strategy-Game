#include "../../include/Transport.hpp"
#include <sstream>

Transport::Transport(const std::string& Name, const Vec2& Pos, Civilization* Owner) :
    Ship(Name, Pos, Owner,
        GameConstants::TRANSPORT_MAX_HEALTH,
        GameConstants::TRANSPORT_MOVEMENT_RANGE,
        GameConstants::TRANSPORT_VISION_RANGE,
        GameConstants::TRANSPORT_ATTACK,
        ShipType::TRANSPORT),
        _cargoCapacity(GameConstants::TRANSPORT_CARGO),
        _cargo(0, 0, 0) {}

std::string Transport::getDetailedInfo() const{
    std::stringstream ss;

    ss << Ship::getDetailedInfo() << std::endl;
    ss << "Cargo: " << _cargo.total() << "/" << _cargoCapacity << std::endl;
    ss << "\tEnergy: " << _cargo.energy << std::endl;
    ss << "\tOre: " << _cargo.ore << std::endl;
    ss << "\tFood: " << _cargo.food << std::endl;

    return ss.str();
}

void Transport::attack(Entity& target, CombatSystem& CombatSystem) {}

void Transport::interactEntity(Entity* other, CombatSystem& combatSystem){
    //This will be added soon, when the Planet class added
}

bool Transport::loadCargo(const Resources& resources){
    if (!isAlive()) return false;
    if (resources.total() <= 0) return false;

    int newTotal = _cargo.total() + resources.total();
    if (newTotal > _cargoCapacity) return false;
    this -> setState(ShipState::LOADING);
    _cargo.energy += resources.energy;
    _cargo.ore += resources.ore;
    _cargo.food += resources.food;

    return true;
}

Resources Transport::unloadCargo(const Resources& resources){
    Resources unloaded(0, 0, 0);

    if (!isAlive()) return unloaded;
    if (resources.energy < 0 || resources.ore < 0 || resources.food < 0) return unloaded;
    if (!(_cargo > resources))  return unloaded;
    this -> setState(ShipState::UNLOADING);
    _cargo.energy -= resources.energy;
    _cargo.ore -= resources.ore;
    _cargo.food -= resources.food;
    unloaded = resources;

    return unloaded; //return the unloaded resources amount (to load it in somewhere else)
}

void Transport::levelUp(){
    _level++;
    _health = _maxHealth += 15;
    if (_level % 5 == 0){
        _movementRange++;
        _visionRange++;
        _cargoCapacity += 400;
    }
    if (_cargoCapacity >= 3000) _cargoCapacity = 3000;
}