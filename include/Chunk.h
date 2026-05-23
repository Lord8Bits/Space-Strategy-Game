#ifndef SPACE_STRATEGY_GAME_SECTOR_H
#define SPACE_STRATEGY_GAME_SECTOR_H

#include <map>
#include <memory>
#include "Entity.hpp"

/// @brief Represents a bounded region of the game world containing entities
///
/// A Chunk is a spatial partition (like Minecraft chunks) that:
/// - Covers a rectangular area defined by world coordinates [_x_start.._x_end] x [_y_start.._y_end]
/// - Stores entities efficiently in a map<entity_id, unique_ptr<Entity>>
/// - Enables O(log n) entity lookups and removals by ID
class Chunk {
private:
    const int _x_start{};  ///< World X where this chunk begins
    const int _x_end{};    ///< World X where this chunk ends
    const int _y_start{};  ///< World Y where this chunk begins
    const int _y_end{};    ///< World Y where this chunk ends

    /// Map of entities by ID for efficient O(log n) lookup and removal
    std::map<const int, std::unique_ptr<Entity>> _entities;

public:
    Chunk(int x_start, int x_end, int y_start, int y_end);
    ~Chunk();

    Chunk(Chunk&& other) noexcept = default;
    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;

    /// @brief Add an entity to this chunk (takes ownership)
    void addEntity(std::unique_ptr<Entity> entity);

    /// @brief Remove an entity from this chunk by ID
    void removeEntity(int entity_id);

    // Getters
    int getXStart() const { return _x_start; }
    int getXEnd()   const { return _x_end;   }
    int getYStart() const { return _y_start; }
    int getYEnd()   const { return _y_end;   }

    /// @brief Read-only access to entities for rendering
    const std::map<const int, std::unique_ptr<Entity>>& getEntities() const { return _entities; }
};

#endif //SPACE_STRATEGY_GAME_SECTOR_H