//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_SECTOR_H
#define SPACE_STRATEGY_GAME_SECTOR_H

#include <map>
#include <memory>
#include "Entity.hpp"

/// @brief Represents a bounded region of the game world containing entities
///
/// A Chunk is a spatial partition (like Minecraft chunks) that:
/// - Covers a rectangular area [_idx_start to _idx_end] in the world buffer
/// - Stores entities efficiently in a map<entity_id, unique_ptr<Entity>>
/// - Enables O(1) entity lookups and removals by ID
class Chunk {
    friend class Render;
private:
    // New (2D world coordinate based)
    const int _x_start{};  // World X where this chunk begins
    const int _x_end{};    // World X where this chunk ends
    const int _y_start{};  // World Y where this chunk begins
    const int _y_end{};    // World Y where this chunk ends

    /// Map of entities by ID for efficient O(log n) lookup and O(log n) removal
    std::map<const int, std::unique_ptr<Entity>> _entities;

public:
    /// @brief Constructor for Chunk
    Chunk(int x_start, int x_end, int y_start, int y_end);

    /// @brief Destructor (automatic cleanup via unique_ptr)
    ~Chunk();

    /// Move constructor - allows Chunk to be moved
    Chunk(Chunk&& other) noexcept = default;

    /// Delete copy constructor - prevent accidental copying
    Chunk(const Chunk&) = delete;

    /// Delete copy assignment - prevent accidental copying
    Chunk& operator=(const Chunk&) = delete;

    /// @brief Add an entity to this chunk
    /// @param entity Unique pointer to Entity (ownership transferred to Chunk) contains its id
    void addEntity(std::unique_ptr<Entity> entity);

    /// @brief Remove an entity from this chunk by ID
    /// @param entity_id ID of entity to remove
    void removeEntity(int entity_id);
};


#endif //SPACE_STRATEGY_GAME_SECTOR_H