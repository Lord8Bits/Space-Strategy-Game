#ifndef SPACE_STRATEGY_GAME_SECTOR_H
#define SPACE_STRATEGY_GAME_SECTOR_H

#include <set>
#include "Entities/Entity.hpp"

/// @brief Represents a bounded region of the game world
///
/// A Chunk is a spatial partition that:
/// - Covers a rectangular area [_x_start.._x_end] x [_y_start.._y_end]
/// - INDEXES entity IDs only (does NOT own entities)
/// - Enables O(log n) membership queries by ID
///
/// Ownership: Map owns all entities. Chunk just tracks which IDs are here.
class Chunk {
private:
    const int _x_start{};  ///< World X where this chunk begins
    const int _x_end{};    ///< World X where this chunk ends
    const int _y_start{};  ///< World Y where this chunk begins
    const int _y_end{};    ///< World Y where this chunk ends

    /// Set of entity IDs present in this chunk (no ownership)
    std::set<int> _entity_ids;

public:
    Chunk(int x_start, int x_end, int y_start, int y_end);
    ~Chunk() = default;

    Chunk(Chunk&& other) noexcept = default;
    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;

    /// @brief Register an entity ID as present in this chunk
    void addEntityIndex(int entity_id);

    /// @brief Remove an entity ID from this chunk's index
    void removeEntityIndex(int entity_id);

    // Getters
    int getXStart() const { return _x_start; }
    int getXEnd()   const { return _x_end;   }
    int getYStart() const { return _y_start; }
    int getYEnd()   const { return _y_end;   }

    /// @brief Read-only access to entity IDs for rendering/queries
    const std::set<int>& getEntityIDs() const { return _entity_ids; }
};

#endif //SPACE_STRATEGY_GAME_SECTOR_H