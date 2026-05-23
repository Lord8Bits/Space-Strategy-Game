#pragma once
#include <string>
#include "Utils/Position.hpp"
#include "Utils/Enums.hpp"
#include "Utils/Constants.hpp"
#include "Civilization.hpp"

/// @brief Cell structure representing a visual element in the game world
struct Cell {
    char symbol;    ///< Character symbol to display
    GameUI::Color color;    ///< Color of the cell
};

class Entity : public Updatable{
protected:
    /// Counter id for each entity.
    /// When a new entity object is initialized, the counter's current value will be applied to _id.
    /// Each initialization increments the counter.
    static int _next_id;

    int _id;

    EntityType _type{};

    std::string _name{"MissingName"};

    /// Position: [ABSOLUTE world coordinates]
    /// Range: [0...VIEWPORT_WIDTH * num_cols) × [0...VIEWPORT_HEIGHT * num_rows)
    /// Note: Independent of which chunk the entity currently belongs to
    Vec2 _position{0,0};

    /// Visual representation of the entity
    Cell _cell{'.', GameUI::Color::WHITE};

    /// Which civilization owns the entity
    Civilization* _owner;

public:
    Entity(EntityType type, std::string  name, const Vec2& pos, Cell cell, Civilization* owner = nullptr);
    ~Entity() = default;
    
    //Pure virtual functions
    virtual void interactEntity(Entity* other) {}
    virtual std::string getDetailedInfo() const { return _name; }
    void update() override {}

    EntityType getType() const;
    int getId() const;
    Cell getCell() const;
    std::string_view getName() const;
    Vec2 getPosition() const;

    void setPosition(const Vec2& pos);
    // Civilization* getCivOwner() const { return _owner; }
    void setCivOwner(Civilization* newOwner);
};