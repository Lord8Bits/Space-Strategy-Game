//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_SECTOR_H
#define SPACE_STRATEGY_GAME_SECTOR_H

#include <map>
#include <vector>
#include <memory>
#include "Entity.h"

/// @brief Represents a bounded region of the game world containing entities
///
/// A Sector is a rectangular area defined by starting and ending coordinates.
/// It manages a collection of SpaceEntity objects within its boundaries.
class Chunk {
    friend class Render;
private:
    const int _idx_start{};
    const int _idx_end{};
    std::map<const int, std::unique_ptr<Entity>> _entities; ///< Entities contained in this sector using a hash table
public:
    /// @brief Constructor for Sector
    /// @param x_s Starting X coordinate
    /// @param x_e Ending X coordinate
    /// @param y_s Starting Y coordinate
    /// @param y_e Ending Y coordinate
    Chunk();

    /// @brief Destructor for Sector
    ~Chunk();

    /// @brief Add an entity to this sector
    /// @param entity Pointer to the SpaceEntity to add
    void addEntity(int entity_id, std::unique_ptr<Entity> entity);

    /// @brief Remove an entity from this sector
    /// @param entity_id key to the Entity to remove
    void removeEntity(int entity_id);
};


#endif //SPACE_STRATEGY_GAME_SECTOR_H