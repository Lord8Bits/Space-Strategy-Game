//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_RENDER_H
#define SPACE_STRATEGY_GAME_RENDER_H
#include <array>
#include "Chunk.h"
#include "Entity.h"
#include "Map.h"

class Map;

/// @brief Handles all rendering of the game world to the terminal
///
/// The Render class is responsible for:
/// - Maintaining a world buffer with visual information
/// - Drawing the current sector to the terminal
/// - Managing color formatting for ANSI terminal output
/// - Translating coordinate systems
class Render {
public:
    static constexpr int VIEWPORT_WIDTH = 80;                                                   ///< Width of visible viewport
    static constexpr int VIEWPORT_HEIGHT = 20;                                                 ///< Height of visible viewport
    static constexpr int SECTOR_COUNT = 6;                                                    ///< Number of Sectors
    static constexpr int WORLD_SIZE = VIEWPORT_HEIGHT*VIEWPORT_WIDTH * SECTOR_COUNT;         ///< Total size of the world buffer
    static constexpr int MAX_BUFFER = WORLD_SIZE * 3;                                       ///< Maximum frame buffer size

private:
    std::array<Cell, WORLD_SIZE> _world{};          ///< World buffer containing all cell data
    std::string _frame_buffer{};                    ///< Current frame to be rendered to screen

public:
    /// @brief Constructor for Render
    /// Initializes the world buffer and frame buffer
    Render();

    /// @brief Destructor for Render
    ~Render();

    /// @brief Render the specified map to the terminal
    ///
    /// This method:
    /// - Clears the viewport
    /// - Places all entities from the selected sector
    /// - Formats the output with color codes
    /// - Sends the frame buffer to stdout
    ///
    /// @param map The Map object containing sectors to render
    void drawWorld(const Map& map);

    /// @brief Translate 2D viewport coordinates to world buffer index
    ///
    /// Converts viewport coordinates (x, y) for a specific sector
    /// into the corresponding index in the world buffer array.
    ///
    /// @param x X coordinate in viewport (0-19)
    /// @param y Y coordinate in viewport (0-19)
    /// @param selected_sector Sector index to calculate offset
    /// @return Index in the _world array
    static int coordTranslation(int x, int y, int selected_sector);

};



#endif //SPACE_STRATEGY_GAME_RENDER_H