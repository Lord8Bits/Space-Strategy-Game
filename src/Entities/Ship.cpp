#include <iostream>
#include <sstream>
#include <algorithm>
#include "../../include/Ship.hpp"
#include "../../include/CombatSystem.hpp"

Ship::Ship(const std::string& Name, const Vec2& Pos, Civilization* Owner, int maxHp, int Movement, int Vision, int Attack, ShipType Type)
    : Entity(Name, Pos, Owner), _health(maxHp), _maxHealth(maxHp), _movementRange(Movement), _visionRange(Vision),
    _attackPower(Attack), _level(1), _xp(0), _entityType(EntityType::SHIP), _shipType(Type), _state(ShipState::IDLE) {}

void Ship::update(){
    if (_health <= 0)
        _state = ShipState::DESTROYED;
}

bool Ship::moveTo(const Vec2& targetPosition){
    if (!isAlive()){
        return false;
    }
    //if the target position > movement range allowed, move will fails
    if (_position.distanceTo(targetPosition) > _movementRange){
        return false;
    }

    _position = targetPosition;
    _state = ShipState::MOVING;

    return true;
}

void Ship::takeDamage(int damage){
    if (damage <= 0 || !isAlive()) return;
    
    _health -= damage;
    _health = std::max(0, _health);
    if (_health <= 0)
        _state = ShipState::DESTROYED;
}


void Ship::attack(Entity& target, CombatSystem& combatSystem){
    combatSystem.resolveCombat(*this, target);
}

void Ship::interactEntity(Entity* other, CombatSystem& combatSystem){
    if (!isAlive() || !other || !other -> isAlive()) return;

    //Check if the other Entity is a Ship, not a Planet
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

    ss << "⇒⇒⇒" << _name << "⇐⇐⇐" << std::endl;
    ss << "Type: ";
    switch(_shipType){
        case ShipType::FIGHTER: ss << "Fighter"; break;
        case ShipType::CRUISER: ss << "Cruiser"; break;
        case ShipType::TRANSPORT: ss << "Transport"; break;
    }
    ss << std::endl;

    ss << "Position: (" << _position.x << ", " << _position.y << ")" << std::endl;
    ss << "Level: " << _level << std::endl;
    ss << "Hp: " << _health << "/" << _maxHealth << std::endl;
    ss << "Xp: " << _xp << "/" << requiredXpForNextLevel() << std::endl;
    ss << "Attack: " << _attackPower << std::endl;
    ss << "Movement: " << _movementRange << std::endl;
    ss << "Vision: " << _visionRange << std::endl;
    
    ss << "state: ";
    switch(_state){
        case ShipState::IDLE: ss << "Idle"; break;
        case ShipState::MOVING: ss << "Moving"; break;
        case ShipState::ATTACKING: ss << "Attacking"; break;
        case ShipState::DESTROYED: ss << "Destroyed"; break;
        case ShipState::LOADING: ss << "Loading"; break;
        case ShipState::UNLOADING: ss << "Unloading"; break;
    }
    
    return ss.str();
}