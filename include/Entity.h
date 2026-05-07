//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_SPACEENTITY_H
#define SPACE_STRATEGY_GAME_SPACEENTITY_H

#include "Civilization.h"
#include "Enums.h"

/// @brief 2D vector structure representing position coordinates
struct Vec2 {
    int x{};  ///< X-coordinate (horizontal position)
    int y{};  ///< Y-coordinate (vertical position)
};

/// @brief Cell structure representing a visual element in the game world
struct Cell {
    char symbol;    ///< Character symbol to display
    GameUI::Color color;    ///< Color of the cell
};

/// @brief Base class for space entities in the game
/// 
/// Represents any entity that exists in the space strategy game world.
/// Entities have a position and a visual representation (cell).
class Entity {
    friend class Render;
private:
    static int _next_id; //Unique id for each entity
    int _id;
    std::string _name;

    /// Position: [ABSOLUTE world coordinates]
    /// Range: [0..WORLD_WIDTH) × [0..WORLD_HEIGHT)
    /// Note: These are independent of which chunk the entity is in
    Vec2 _pos{0,0};

    Cell _cell{'.', GameUI::Color::WHITE};      ///< Visual representation of the entity
public:
    /// @brief Constructor for SpaceEntity
    /// @param new_pos Initial position of the entity
    /// @param new_cell Visual representation of the entity
    Entity(Vec2 new_pos, Cell new_cell);
    
    /// @brief Virtual destructor for proper cleanup of derived classes
    virtual ~Entity();
};


#endif //SPACE_STRATEGY_GAME_SPACEENTITY_H