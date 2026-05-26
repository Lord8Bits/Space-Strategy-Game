#pragma once
#include <string>
#include "Utils/Position.hpp"
#include "Utils/Enums.hpp"
#include "Utils/Constants.hpp"
#include "Updatable.hpp"
#include "Civilization.hpp"

/// @brief Base class for all game entities (ships, planets, etc.)
///
/// Entity stores GAMEPLAY data only:
///   - Identity (id, name)
///   - Position (world coordinates)
///   - Visual hint (symbol char - the 'what am I' aspect)
///   - Ownership (which civilization controls this)
///   - Behavior (pure virtual update/interact)
///
/// Rendering data (Cell, colors) is built by Render::makeCell()
/// using getSymbol() + getCivOwner()->getColor()
class Entity : public Updatable {
protected:
    static int _next_id;

    int _id;
    std::string _name{"MissingName"};

    /// @brief Absolute world coordinates
    Vec2 _position{0, 0};

    /// @brief Single character representing this entity type visually
    /// Example: 'S' for ship, 'P' for planet
    char _symbol{'?'};

    /// @brief Non-owning pointer to the civilization that controls this entity
    /// nullptr means unclaimed/neutral
    Civilization* _owner{nullptr};

public:
    Entity(std::string name, const Vec2& pos, char symbol, Civilization* owner = nullptr);
    ~Entity() = default;

    void update() override {}

    // Getters
    int              getId()       const { return _id; }
    std::string_view getName()     const { return _name; }
    Vec2             getPosition() const { return _position; }
    char             getSymbol()   const { return _symbol; }
    Civilization*    getCivOwner() const { return _owner; }

    // Setters
    void setPosition(const Vec2& pos);
    void setCivOwner(Civilization* newOwner);
};