#include "Entities/Transport.hpp"
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

bool Transport::loadCargo(const Resource& resource)
{
    if (!isAlive()) return false;
    if (resource.total() <= 0) return false;

    // Reject if loading this would exceed capacity
    if (_cargo.total() + resource.total() > _cargoCapacity) return false;

    setState(ShipState::LOADING);
    _cargo += resource;  // operator+= handles the addition
    return true;
}

Resource Transport::unloadCargo(const Resource& requested)
{
    // Return empty resource as a "nothing transferred" signal
    if (!isAlive()) return {0, 0, 0};

    // Can the cargo cover what was requested?
    if (!_cargo.canAfford(requested)) return {0, 0, 0};

    setState(ShipState::UNLOADING);
    _cargo -= requested;        // subtract from cargo
    return requested;           // return the transferred amount to the caller
}

CombatResult Transport::attack(Entity& /*target*/, CombatSystem& /*combatSystem*/)
{
    CombatResult result;
    result.logs.push_back({0, getName() + " cannot attack — transports are unarmed."});
    return result;
}

void Transport::interactEntity(Entity* other, CombatSystem& combatSystem)
{
    // Will be implemented when Planet class is added
}

void Transport::levelUp()
{
    _level++;
    _health = _maxHealth += 15;
    if (_level % 5 == 0){
        _movementRange++;
        _visionRange++;
        _cargoCapacity += 400;
    }
    if (_cargoCapacity >= 3000) _cargoCapacity = 3000;
}

std::string Transport::getDetailedInfo() const
{
    std::stringstream ss;
    ss << Ship::getDetailedInfo()                              << "\n";
    ss << "Cargo: " << _cargo.total() << "/" << _cargoCapacity << "\n";
    ss << "  Gold:     " << _cargo.getGold()     << "\n";
    ss << "  Titanium: " << _cargo.getTitanium() << "\n";
    ss << "  Cadmium:  " << _cargo.getCadmium()  << "\n";
    return ss.str();
}