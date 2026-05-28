#ifndef SPACE_STRATEGY_GAME_MAP_H
#define SPACE_STRATEGY_GAME_MAP_H

#include <vector>
#include <unordered_map>
#include <memory>
#include <set>
#include "Chunk.hpp"
#include "Entity.hpp"

/// @brief Manages the entire game world and owns all entities
///
/// Ownership model:
///   Map       → owns all entities (unique_ptr in _all_entities)
///   Chunk     → indexes entity IDs for spatial queries (no ownership)
///   Civilization → stores entity IDs for gameplay/AI (no ownership)
///
/// This enables:
///   - Ship movement: just update chunk index, no ownership transfer
///   - Entity death:  remove from Map, chunk indices auto-invalidate
///   - Clean API:     one place to create/destroy entities
class Map {
private:
    /// Single source of truth for entity lifetime
    std::unordered_map<int, std::unique_ptr<Entity>> _all_entities;

    /// Spatial partitioning grid - chunks index entity IDs only
    std::vector<Chunk> _chunks;

    const int _chunk_row;  ///< Number of chunk rows
    const int _chunk_col;  ///< Number of chunk columns
    int _selected_chunk{}; ///< Index of currently viewed chunk

public:
    /// @brief Construct a world with num_rows × num_cols chunks (auto-generated)
    Map(int num_rows, int num_cols);
    ~Map();

    /// @brief Add an entity to the world (Map takes ownership)
    /// Automatically places entity in correct chunk based on position
    /// @param entity Unique pointer to entity
    /// @return Assigned entity ID for future reference
    /// @throws std::out_of_range if position is outside world bounds
    int addEntity(std::unique_ptr<Entity> entity);

    /// @brief Permanently remove and destroy an entity
    /// Removes from chunk index and central registry
    /// @param entity_id ID of the entity to delete
    void removeEntity(int entity_id);

    /// @brief Get a non-owning pointer to an entity by ID
    /// @return Entity pointer, or nullptr if not found
    Entity* getEntity(int entity_id);
    const Entity* getEntity(int entity_id) const;

    /// @brief Re-index an entity after it moved to a new position
    /// Call this after setPosition() to keep chunk membership in sync
    /// @param entity_id ID of the entity that moved
    /// @throws std::out_of_range if new position is outside world bounds
    void updateEntityChunk(int entity_id);

    /// @brief Find which chunk index contains a world position (O(1))
    /// @throws std::out_of_range if position is outside world bounds
    int findChunkIndex(const Vec2& world_pos) const;

    /// @brief Get the currently selected Chunk
    const Chunk& getSelectedChunk() const;

    /// @brief Switch active chunk using grid coordinates
    /// @param chunk_x Column index (0-based)
    /// @param chunk_y Row index (0-based)
    /// @throws std::out_of_range if grid coords are invalid
    void changeSelectedChunk(int chunk_x, int chunk_y);

    // World info getters
    int getSelectedChunkIndex() const { return _selected_chunk; }
    int getChunkRows()          const { return _chunk_row; }
    int getChunkCols()          const { return _chunk_col; }
    int getWorldWidth()         const { return _chunk_col * VIEWPORT_WIDTH; }
    int getWorldHeight()        const { return _chunk_row * VIEWPORT_HEIGHT; }
};

#endif //SPACE_STRATEGY_GAME_MAP_H