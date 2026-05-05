//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_SECTOR_H
#define SPACE_STRATEGY_GAME_SECTOR_H

#include <vector>

#include "SpaceEntity.h"

/// @brief Represents a bounded region of the game world containing entities
///
/// A Sector is a rectangular area defined by starting and ending coordinates.
/// It manages a collection of SpaceEntity objects within its boundaries.
class Sector {
    friend class Render;
private:
    int _x_start;                       ///< Starting X coordinate of the sector
    int _x_end;                         ///< Ending X coordinate of the sector
    int _y_start;                       ///< Starting Y coordinate of the sector
    int _y_end;                         ///< Ending Y coordinate of the sector
    std::vector<SpaceEntity*> _entities; ///< Entities contained in this sector
public:
    /// @brief Constructor for Sector
    /// @param x_s Starting X coordinate
    /// @param x_e Ending X coordinate
    /// @param y_s Starting Y coordinate
    /// @param y_e Ending Y coordinate
    Sector(int x_s, int x_e, int y_s, int y_e);

    /// @brief Destructor for Sector
    ~Sector();

    /// @brief Add an entity to this sector
    /// @param entity Pointer to the SpaceEntity to add
    void addEntity(SpaceEntity* entity);

    /// @brief Remove an entity from this sector
    /// @param entity Pointer to the SpaceEntity to remove
    void removeEntity(SpaceEntity* entity);
};


#endif //SPACE_STRATEGY_GAME_SECTOR_H