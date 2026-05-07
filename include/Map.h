//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_MAP_H
#define SPACE_STRATEGY_GAME_MAP_H
#include <vector>

#include "Sector.h"

/// @brief World layout and chunk mapping
///
/// Example: If world is 80×40 cells divided into 2 chunks:
/// - Chunk 0: world buffer [0..1600) → covers world (0,0) to (80,20)
/// - Chunk 1: world buffer [1600..3200) → covers world (0,20) to (80,40)
///
/// Entity at world pos (45, 25) belongs to chunk 1
/// Relative to chunk: x=45, y=5 (offset from chunk start)
class Map {
    friend class Render;
private:
    std::vector<Chunk> _sectors;   ///< Collection of all sectors in the game world
    int _selected_sector{};         ///< Index of the currently active sector
public:
    /// @brief Constructor for Map
    Map();

    /// @brief Destructor for Map
    ~Map();

    /// Find which chunk contains an entity at world position pos
    int findChunk(const Vec2& pos) const;

    /// @brief Add a sector to the map
    /// @param sector The Sector to add to the map
    void addSector(const Chunk& sector);
};


#endif //SPACE_STRATEGY_GAME_MAP_H