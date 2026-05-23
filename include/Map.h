//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_MAP_H
#define SPACE_STRATEGY_GAME_MAP_H
#include <vector>

#include "Chunk.h"

/// @brief World layout and chunk mapping
///
/// Example: If world is 80×40 cells divided into 2 chunks:
/// - Chunk 0: world buffer [0..1600) → covers world (0,0) to (80,20)
/// - Chunk 1: world buffer [1600..3200) → covers world (0,20) to (80,40)
///
/// Entity at world pos (45, 25) belongs to chunk 1
/// Relative to chunk: x=45, y=5 (offset from chunk start)
class Map {
private:
    std::vector<Chunk> _chunks;   ///< Collection of all sectors in the game world
    const int _chunk_row;
    const int _chunk_col;
    int _selected_chunk{};         ///< Index of the currently active sector
public:
    /// @brief Constructor for Map
    Map(int num_rows, int num_cols);

    /// @brief Destructor for Map
    ~Map();

    /// Find which chunk contains an entity at world position pos
    //int findChunk(const Vec2& pos) const;

    /// @brief Add an entity to the map (automatically places in correct chunk)
    /// @param entity Unique pointer to entity (ownership transferred)
    /// @throws std::out_of_range if entity position is outside world bounds
    void addEntity(std::unique_ptr<Entity> entity);

    const Chunk& getSelectedChunk() const;
    int findChunkIndex(const Vec2& world_pos) const;
    void changeSelectedChunk(int chunk_x, int chunk_y);
};


#endif //SPACE_STRATEGY_GAME_MAP_H