#pragma once
#include <string>
#include "Utils/Position.hpp"
#include "Utils/Enums.hpp"
#include "AI/Civilization.hpp"

class CombatSystem;
class Ship;
class Planet;

/// @brief Abstract base class for every object that exists in the game world.
///
/// All game entities — ships, planets — inherit from Entity.
/// Entity defines the common identity (id, name, position, owner) and a
/// rich virtual interface so that systems like CombatSystem, Render, and the
/// AI can operate on any entity through this base pointer without knowing
/// the concrete type.
///
/// Design note — self-cast pattern:
///   Instead of dynamic_cast, each concrete subclass overrides asShip() or
///   asPlanet() to return `this`.  The base implementation returns nullptr.
///   Callers check the pointer rather than using RTTI, which is safer and
///   avoids the overhead of dynamic_cast at every combat / render call.
class Entity : public Updatable {
protected:
    static int  _next_id;   ///< Global auto-increment counter shared by all entities
    int         _id;        ///< Unique identifier assigned at construction
    std::string _name;      ///< Display name shown in the UI
    Vec2        _position;  ///< Current world-space coordinates
    Civilization* _owner;   ///< Non-owning pointer to the controlling civilization (may be null)

public:
    /// @param Name  Display name of the entity
    /// @param Pos   Starting world position
    /// @param Owner Controlling civilization, or nullptr for neutral entities
    Entity(const std::string& Name, const Vec2& Pos, Civilization* Owner = nullptr);
    virtual ~Entity() = default;

    // ── Pure-virtual interface — every concrete entity must implement these ───

    /// @brief Single UTF-8 character used to draw this entity on the map.
    virtual std::string   getSymbol()       const = 0;

    /// @brief Broad category tag (SHIP or PLANET) used for type dispatching.
    virtual EntityType    getType()         const = 0;

    /// @brief Called when two entities occupy the same tile; ships auto-attack enemies.
    virtual void          interactEntity(Entity* other, CombatSystem& cs) = 0;

    /// @brief Multi-line string shown by the `status <id>` command.
    virtual std::string   getDetailedInfo() const = 0;

    /// @brief Returns false when the entity has been destroyed / depleted.
    virtual bool          isAlive()         const = 0;

    /// @brief Apply raw damage after shield absorption.
    virtual void          takeDamage(int damage)  = 0;

    /// @brief Experience reward granted to the attacker on kill.
    virtual int           getXpReward()     const = 0;

    // ── Self-cast — replaces dynamic_cast throughout the codebase ────────────
    /// @brief Returns `this` if the entity is a Ship; nullptr otherwise.
    virtual Ship*         asShip()         { return nullptr; }
    virtual const Ship*   asShip()   const { return nullptr; }

    /// @brief Returns `this` if the entity is a Planet; nullptr otherwise.
    virtual Planet*       asPlanet()       { return nullptr; }
    virtual const Planet* asPlanet() const { return nullptr; }

    // ── Stat virtuals — queried by UI and combat without knowing the type ─────
    virtual int  getHealth()        const { return 0; }     ///< Current hit points
    virtual int  getMaxHealth()     const { return 0; }     ///< Maximum hit points
    virtual int  getAttackPower()   const { return 0; }     ///< Base damage per attack
    virtual int  getResourceCount() const { return 0; }     ///< Total resources stored (planets)
    virtual bool isColonized()      const { return false; } ///< True if planet is owned
    virtual bool canCounterAttack() const { return false; } ///< True for melee-capable ships

    // ── Fog-of-war contribution ───────────────────────────────────────────────
    /// @brief Radius (Manhattan distance) of tiles this entity reveals.
    /// Ships return their vision range; planets return 51 when colonized (full sector).
    /// Returns 0 for entities that do not contribute to visibility.
    virtual int getVisionRange() const { return 0; }

    // ── Rendering color ───────────────────────────────────────────────────────
    /// @brief ANSI color used when drawing this entity on the map.
    /// Defaults to the owner civilization's color; subclasses may override
    /// (e.g. Planet uses type-based color when uncolonized).
    virtual GameUI::Color getDisplayColor() const {
        return _owner ? _owner->getColor() : GameUI::Color::WHITE;
    }

    // ── Combat participation ──────────────────────────────────────────────────
    /// @brief Returns false for entities that cannot be targeted (e.g. Planet).
    virtual bool canBeAttacked()                const { return true; }

    /// @brief Roll dodge chance; returns true if the attack should be negated.
    virtual bool tryDodge(const Ship& attacker) const { return false; }

    /// @brief Apply shield/armor reduction; returns remaining damage after absorption.
    virtual int  absorbDamage(int damage)             { return damage; }

    // ── Accessors ─────────────────────────────────────────────────────────────
    int           getId()       const { return _id;       }
    std::string   getName()     const { return _name;     }
    Vec2          getPosition() const { return _position; }
    void          setPosition(const Vec2& pos)  { _position = pos; }
    Civilization* getCivOwner() const { return _owner;    }
    void          setCivOwner(Civilization* c)  { _owner = c; }
};
