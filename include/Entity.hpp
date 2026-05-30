#pragma once
#include <string>
#include "../src/Utils/Position.hpp"
#include "../src/Utils/Enums.hpp"
#include "Civilization.hpp"

class CombatSystem;
class Ship;
class Planet;

class Entity : public Updatable {
protected:
    static int  _next_id;
    int         _id;
    std::string _name;
    Vec2        _position;
    Civilization* _owner;

public:
    Entity(const std::string& Name, const Vec2& Pos, Civilization* Owner = nullptr);
    virtual ~Entity() = default;

    // ── Pure-virtual interface ────────────────────────────────────────────────
    virtual std::string   getSymbol()       const = 0;
    virtual EntityType    getType()         const = 0;
    virtual void          interactEntity(Entity* other, CombatSystem& cs) = 0;
    virtual std::string   getDetailedInfo() const = 0;
    virtual bool          isAlive()         const = 0;
    virtual void          takeDamage(int damage)  = 0;
    virtual int           getXpReward()     const = 0;

    // ── Self-cast (avoids dynamic_cast; subclass returns `this`, base returns nullptr) ──
    virtual Ship*         asShip()         { return nullptr; }
    virtual const Ship*   asShip()   const { return nullptr; }
    virtual Planet*       asPlanet()       { return nullptr; }
    virtual const Planet* asPlanet() const { return nullptr; }

    // ── Stat virtuals (used by UI / combat without knowing concrete type) ─────
    virtual int  getHealth()         const { return 0; }
    virtual int  getMaxHealth()      const { return 0; }
    virtual int  getAttackPower()    const { return 0; }
    virtual int  getResourceCount()  const { return 0; }
    virtual bool isColonized()       const { return false; }
    virtual bool canCounterAttack()  const { return false; }

    // ── Vision (FoW update; non-mobile entities return 0) ────────────────────
    virtual int getVisionRange() const { return 0; }

    // ── Display color (override for type-based coloring, e.g. Planet) ────────
    virtual GameUI::Color getDisplayColor() const {
        return _owner ? _owner->getColor() : GameUI::Color::WHITE;
    }

    // ── Combat participation ──────────────────────────────────────────────────
    virtual bool canBeAttacked()                      const { return true; }
    virtual bool tryDodge(const Ship& attacker)       const { return false; }
    virtual int  absorbDamage(int damage)                   { return damage; }

    // ── Accessors ─────────────────────────────────────────────────────────────
    int           getId()        const { return _id;       }
    std::string   getName()      const { return _name;     }
    Vec2          getPosition()  const { return _position; }
    void          setPosition(const Vec2& pos) { _position = pos; }
    Civilization* getCivOwner()  const { return _owner;    }
    void          setCivOwner(Civilization* c) { _owner = c; }
};
