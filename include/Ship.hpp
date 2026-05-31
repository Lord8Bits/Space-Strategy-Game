#pragma once
#include "Enums.hpp"
#include "Entity.hpp"
#include "CombatResult.hpp"

class CombatSystem;

class Ship : public Entity {
protected:
    int _health;
    int _maxHealth;
    int _movementRange;   ///< Max movement points per turn
    int _movementPoints;  ///< Current budget this turn (resets each turn)
    int _visionRange;
    int _attackPower;
    int _attackRange;     ///< Max tiles away the ship can attack (1 = melee)
    int _level;
    int _xp;
    EntityType _entityType;
    ShipType   _shipType;
    ShipState  _state;

    Vec2 _destination;
    bool _hasDestination;

public:
    Ship(const std::string& Name, const Vec2& Pos, Civilization* Owner,
         int maxHp, int Movement, int Vision, int Attack, ShipType Type);
    virtual ~Ship() = default;

    // ── Entity interface ──────────────────────────────────────────────────────
    void update() override;
    std::string getDetailedInfo() const override;
    void takeDamage(int damage) override;
    void interactEntity(Entity* other, CombatSystem& cs) override;
    EntityType getType() const override { return _entityType; }
    bool isAlive() const override { return _health > 0; }

    // ── Self-cast ─────────────────────────────────────────────────────────────
    Ship*       asShip()       override { return this; }
    const Ship* asShip() const override { return this; }

    // ── Stat virtuals overridden from Entity ──────────────────────────────────
    int getHealth()      const override { return _health;      }
    int getMaxHealth()   const override { return _maxHealth;   }
    int getAttackPower() const override { return _attackPower; }
    int getVisionRange() const override { return _visionRange; }

    // ── Ship-specific virtuals ────────────────────────────────────────────────
    virtual bool          moveTo(const Vec2& target);
    virtual CombatResult  attack(Entity& target, CombatSystem& cs);
    virtual int           calculateDamageAgainst(const Entity& target) const { return _attackPower; }
    virtual ShipType      getShipType() const = 0;
    virtual void          levelUp()           = 0;
    virtual bool          canMine()     const { return false; }

    // ── Movement ──────────────────────────────────────────────────────────────
    void setDestination(const Vec2& dest) {
        _destination = dest; _hasDestination = true; _state = ShipState::MOVING;
    }
    bool advanceTowardDestination();
    /// Called each turn after AI acts — auto-advances ships still mid-travel.
    void advancePendingMovement();
    /// Cancels any pending movement or action and returns the ship to Idle.
    void cancelAction();
    bool hasDestination()     const { return _hasDestination; }
    Vec2 getDestination()     const { return _destination; }
    bool reachedDestination() const { return _position == _destination; }

    // ── Movement points ───────────────────────────────────────────────────────
    int  getMovementPoints()     const { return _movementPoints; }
    int  getMovementRange()      const { return _movementRange; }
    void setMovementPoints(int v)      { _movementPoints = v; }
    int  getAttackRange()        const { return _attackRange; }
    bool canAttack()             const { return _movementPoints > 0 && isAlive(); }

    // ── XP / level ────────────────────────────────────────────────────────────
    void gainXp(int amount);
    int  getLevel() const { return _level; }
    int  getXp()    const { return _xp; }
    int  requiredXpForNextLevel() const { return _level * 100; }

    // ── State ─────────────────────────────────────────────────────────────────
    ShipState getState()         const { return _state; }
    void setState(ShipState s)         { _state = s; }
};
