//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_MAP_H
#define SPACE_STRATEGY_GAME_MAP_H
#include <vector>

#include "Sector.h"

/// @brief Manages the entire game world composed of multiple sectors
///
/// The Map class is responsible for organizing the game world into sectors
/// and tracking which sector is currently selected for rendering and interaction.
class Map {
    friend class Render;
private:
    std::vector<Sector> _sectors;   ///< Collection of all sectors in the game world
    int _selected_sector{};         ///< Index of the currently active sector
public:
    /// @brief Constructor for Map
    Map();

    /// @brief Destructor for Map
    ~Map();

    /// @brief Add a sector to the map
    /// @param sector The Sector to add to the map
    void addSector(const Sector& sector);
};


#endif //SPACE_STRATEGY_GAME_MAP_H