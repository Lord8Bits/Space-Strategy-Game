#include <sstream>
#include <algorithm>
#include "Entities/Ship.hpp"
#include "Systems/CombatSystem.hpp"

Ship::Ship(const std::string& Name, const Vec2& Pos, Civilization* Owner,
           int maxHp, int Movement, int Vision, int Attack, ShipType Type)
    : Entity(Name, Pos, Owner)
    , _health(maxHp), _maxHealth(maxHp)
    , _movementRange(Movement), _movementPoints(Movement)
    , _visionRange(Vision), _attackPower(Attack)
    , _attackRange(1)
    , _level(1), _xp(0)
    , _entityType(EntityType::SHIP), _shipType(Type)
    , _state(ShipState::IDLE)
    , _destination(Pos), _hasDestination(false)
{}

void Ship::update() {
    if (_health <= 0) { _state = ShipState::DESTROYED; return; }
    _movementPoints = _movementRange;   // reset movement budget each turn
    if (_state == ShipState::ATTACKING) _state = ShipState::IDLE;
    // Auto-movement is handled by advancePendingMovement(), called from
    // Game::advanceTurn() AFTER AI acts — so AI always gets full MP.
}

void Ship::advancePendingMovement() {
    if (!isAlive() || !_hasDestination || reachedDestination()) return;
    advanceTowardDestination();
}

void Ship::cancelAction() {
    _hasDestination = false;
    _destination    = _position;
    if (_state != ShipState::DESTROYED)
        _state = ShipState::IDLE;
}

bool Ship::moveTo(const Vec2& target) {
    if (!isAlive() || _movementPoints <= 0) return false;
    const int dist = _position.distanceTo(target);
    if (dist > _movementPoints) return false;
    _movementPoints -= dist;
    _position = target;
    _state = (_movementPoints > 0) ? ShipState::IDLE : ShipState::MOVING;
    return true;
}

bool Ship::advanceTowardDestination() {
    if (!isAlive() || !_hasDestination || _movementPoints <= 0) return false;
    if (reachedDestination()) {
        _hasDestination = false;
        _state = ShipState::IDLE;
        return false;
    }

    int budget = _movementPoints;
    Vec2 pos   = _position;

    while (budget > 0 && pos != _destination) {
        if      (pos.x != _destination.x) pos.x += (pos.x < _destination.x) ? 1 : -1;
        else if (pos.y != _destination.y) pos.y += (pos.y < _destination.y) ? 1 : -1;
        --budget;
    }

    _movementPoints = budget;
    const bool moved = (pos != _position);
    _position = pos;

    if (reachedDestination()) { _hasDestination = false; _state = ShipState::IDLE; }
    else if (moved)            { _state = ShipState::MOVING; }
    return moved;
}

void Ship::takeDamage(int damage) {
    if (damage <= 0 || !isAlive()) return;
    _health = std::max(0, _health - damage);
    if (_health <= 0) _state = ShipState::DESTROYED;
}

CombatResult Ship::attack(Entity& target, CombatSystem& cs) {
    return cs.resolveCombat(*this, target);
}

void Ship::interactEntity(Entity* other, CombatSystem& cs) {
    if (!isAlive() || !other || !other->isAlive()) return;
    Ship* otherShip = other->asShip();   // no dynamic_cast
    if (!otherShip) return;
    Civilization* owner      = getCivOwner();
    Civilization* otherOwner = otherShip->getCivOwner();
    if (!owner || !otherOwner || owner == otherOwner) return;
    if (owner->getRelationWith(*otherOwner) == Relation::ENEMY)
        attack(*other, cs);
}

void Ship::gainXp(int amount) {
    if (amount <= 0) return;
    _xp += amount;
    while (_xp >= requiredXpForNextLevel()) {
        _xp -= requiredXpForNextLevel();
        levelUp();
    }
}

std::string Ship::getDetailedInfo() const {
    std::stringstream ss;
    ss << _name << " [" << _id << "]  ";
    switch (_shipType) {
        case ShipType::FIGHTER:   ss << "Fighter";   break;
        case ShipType::CRUISER:   ss << "Cruiser";   break;
        case ShipType::TRANSPORT: ss << "Transport"; break;
    }
    ss << "  HP "  << _health << "/" << _maxHealth
       << "  ATK " << _attackPower
       << "  MP "  << _movementPoints << "/" << _movementRange
       << "  Lvl " << _level;
    ss << "  [";
    switch (_state) {
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
