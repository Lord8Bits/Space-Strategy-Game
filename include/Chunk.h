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
/// A Chunk is a spatial partition (like Minecraft chunks) that:
/// - Covers a rectangular area [_idx_start to _idx_end] in the world buffer
/// - Stores entities efficiently in a map<entity_id, unique_ptr<Entity>>
/// - Enables O(1) entity lookups and removals by ID
class Chunk {
    friend class Render;
private:
    /// Starting index in the world buffer (absolute position in _world array)
    const int _idx_start{};

    /// Ending index in the world buffer
    const int _idx_end{};

    /// Map of entities by ID for efficient O(log n) lookup and O(log n) removal
    std::map<const int, std::unique_ptr<Entity>> _entities;

public:
    /// @brief Constructor for Chunk
    Chunk(int idx_start, int idx_end);

    /// @brief Destructor (automatic cleanup via unique_ptr)
    ~Chunk();

    /// Move constructor - allows Chunk to be moved
    Chunk(Chunk&& other) noexcept = default;

    /// Delete copy constructor - prevent accidental copying
    Chunk(const Chunk&) = delete;

    /// Delete copy assignment - prevent accidental copying
    Chunk& operator=(const Chunk&) = delete;

    /// @brief Add an entity to this chunk
    /// @param entity_id Unique identifier for the entity
    /// @param entity Unique pointer to Entity (ownership transferred to Chunk)
    void addEntity(std::unique_ptr<Entity> entity);

    /// @brief Remove an entity from this chunk by ID
    /// @param entity_id ID of entity to remove
    void removeEntity(int entity_id);
};


#endif //SPACE_STRATEGY_GAME_SECTOR_H