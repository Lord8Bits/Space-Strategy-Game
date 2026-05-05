//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_SPACEENTITY_H
#define SPACE_STRATEGY_GAME_SPACEENTITY_H

#include <string>

/// @brief 2D vector structure representing position coordinates
struct Vec2 {
    int x{};  ///< X-coordinate (horizontal position)
    int y{};  ///< Y-coordinate (vertical position)
};

/// @brief Color enumeration for terminal color output
enum Color {
    White,  ///< Default white/reset color
    Red,    ///< Red color
    Blue,   ///< Blue color
    Green,  ///< Green color
};

/// @brief Cell structure representing a visual element in the game world
struct Cell {
    char symbol;    ///< Character symbol to display
    Color color;    ///< Color of the cell
};

/// @brief Base class for space entities in the game
/// 
/// Represents any entity that exists in the space strategy game world.
/// Entities have a position and a visual representation (cell).
class SpaceEntity {
    friend class Render;
private:
    Vec2 _pos{0,0};              ///< Current position of the entity
    Cell _cell{'.', White};      ///< Visual representation of the entity
public:
    /// @brief Constructor for SpaceEntity
    /// @param new_pos Initial position of the entity
    /// @param new_cell Visual representation of the entity
    SpaceEntity(Vec2 new_pos, Cell new_cell);
    
    /// @brief Virtual destructor for proper cleanup of derived classes
    virtual ~SpaceEntity();
};


#endif //SPACE_STRATEGY_GAME_SPACEENTITY_H