#pragma once
#include "../src/Utils/Enums.hpp"
#include "Entity.hpp"

class CombatSystem; //instead of #include "CombatSystem.hpp" to avoid the circular include error.

class Ship : public Entity{
protected:
    int _health;
    int _maxHealth;
    int _movementRange;
    int _visionRange;
    int _attackPower;
    int _level;
    int _xp;
    EntityType _entityType;
    ShipType _shipType;
    ShipState _state;

public:
    Ship(const std::string& Name, const Vec2& Pos, Civilization* Owner, int maxHp, int Movement, int Vision, int Attack, ShipType Type);
    virtual ~Ship() = default;

    void update() override;
    void gainXp(int amount);
    std::string getDetailedInfo() const override;
    
    virtual bool moveTo(const Vec2& targetPosition);
    virtual void attack(Entity& target, CombatSystem& CombatSystem);
    void takeDamage(int damage) override;
    void interactEntity(Entity* other, CombatSystem& combatSystem) override;
    virtual int calculateDamageAgainst(const Entity& target) const{ return _attackPower; }
    virtual ShipType getShipType() const = 0;
    virtual void levelUp() = 0;

    int getHealth() const { return _health; }
    int getMaxHealth() const { return _maxHealth; }
    int getMovementRange() const { return _movementRange; }
    int getVisionRange() const { return _visionRange; }
    int getAttackPower() const { return _attackPower; }
    int getLevel() const { return _level; }
    int getXp() const { return _xp; }
    EntityType getType() const override { return _entityType; }
    ShipState getState() const { return _state; }
    void setState(ShipState newState) { _state = newState; }
    bool isAlive() const override { return _health > 0; }
    int requiredXpForNextLevel() const { return _level * 100; }
};