#include <iostream>
#include <sstream>
#include <algorithm>
#include "../../include/Ship.hpp"
#include "../../include/CombatSystem.hpp"

Ship::Ship(const std::string& Name, const Vec2& Pos, Civilization* Owner, int maxHp, int Movement, int Vision, int Attack, ShipType Type)
    : Entity(Name, Pos, Owner), _health(maxHp), _maxHealth(maxHp), _movementRange(Movement), _visionRange(Vision),
    _attackPower(Attack), _level(1), _xp(0), _entityType(EntityType::SHIP), _shipType(Type), _state(ShipState::IDLE),
    _destination(Pos), _hasDestination(false) {}

void Ship::update(){
    if (_health <= 0) {
        _state = ShipState::DESTROYED;
        return;
    }
    // If travelling, keep advancing toward the destination each turn
    if (_hasDestination && !reachedDestination()) {
        advanceTowardDestination();
    }
}

bool Ship::moveTo(const Vec2& targetPosition){
    if (!isAlive()) return false;
    if (_position.distanceTo(targetPosition) > _movementRange) return false;

    _position = targetPosition;
    _state = ShipState::MOVING;
    return true;
}

bool Ship::advanceTowardDestination(){
    if (!isAlive() || !_hasDestination) return false;
    if (reachedDestination()) {
        _hasDestination = false;
        _state = ShipState::IDLE;
        return false;
    }

    // Greedy step: spend up to _movementRange points moving along X then Y (Manhattan)
    int budget = _movementRange;
    Vec2 pos = _position;

    while (budget > 0 && pos != _destination) {
        if (pos.x != _destination.x) {
            pos.x += (_destination.x > pos.x) ? 1 : -1;
        } else if (pos.y != _destination.y) {
            pos.y += (_destination.y > pos.y) ? 1 : -1;
        }
        --budget;
    }

    const bool moved = (pos != _position);
    _position = pos;

    if (reachedDestination()) {
        _hasDestination = false;
        _state = ShipState::IDLE;   // arrived — awaiting orders
    } else {
        _state = ShipState::MOVING;
    }
    return moved;
}

void Ship::takeDamage(int damage){
    if (damage <= 0 || !isAlive()) return;
    _health -= damage;
    _health = std::max(0, _health);
    if (_health <= 0)
        _state = ShipState::DESTROYED;
}

CombatResult Ship::attack(Entity& target, CombatSystem& combatSystem){
    return combatSystem.resolveCombat(*this, target);
}

void Ship::interactEntity(Entity* other, CombatSystem& combatSystem){
    if (!isAlive() || !other || !other -> isAlive()) return;
    Ship* otherEntity = dynamic_cast<Ship*>(other);
    if (otherEntity){
        Civilization* owner = getCivOwner();
        Civilization* otherOwner = otherEntity -> getCivOwner();
        if (!owner || !otherOwner || owner == otherOwner) return;
        if (owner -> getRelationWith(*otherOwner) == Relation::ENEMY)
            attack(*other, combatSystem);
    }
}

void Ship::gainXp(int amount){
    if (amount <= 0) return;
    _xp += amount;
    while (_xp >= requiredXpForNextLevel()){
        _xp -= requiredXpForNextLevel();
        levelUp();
    }
}

std::string Ship::getDetailedInfo() const{
    std::stringstream ss;
    ss << _name << " [" << _id << "]  ";
    switch(_shipType){
        case ShipType::FIGHTER:   ss << "Fighter";   break;
        case ShipType::CRUISER:   ss << "Cruiser";   break;
        case ShipType::TRANSPORT: ss << "Transport"; break;
    }
    ss << "  pos(" << _position.x << "," << _position.y << ")"
       << "  HP " << _health << "/" << _maxHealth
       << "  ATK " << _attackPower
       << "  MOV " << _movementRange;

    ss << "  [";
    switch(_state){
        case ShipState::IDLE:      ss << "Idle";      break;
        case ShipState::MOVING:    ss << "Moving";    break;
        case ShipState::ATTACKING: ss << "Attacking"; break;
        case ShipState::DESTROYED: ss << "Destroyed"; break;
        case ShipState::LOADING:   ss << "Loading";   break;
        case ShipState::UNLOADING: ss << "Unloading"; break;
    }
    ss << "]";
    if (_hasDestination)
        ss << " -> (" << _destination.x << "," << _destination.y << ")";
    return ss.str();
}