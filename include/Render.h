#ifndef SPACE_STRATEGY_GAME_RENDER_H
#define SPACE_STRATEGY_GAME_RENDER_H
#include <array>
#include "Entity.hpp"
#include "Map.h"

/// @brief Handles all rendering of the game world to the terminal
///
/// The Render class is responsible for:
/// - Maintaining a viewport buffer for the SELECTED chunk only (not the entire world)
/// - Drawing the current chunk to the terminal with ANSI color codes
/// - Converting world coordinates to screen coordinates
///
/// Design decision: _viewport only stores one chunk at a time (VIEWPORT_SIZE cells)
/// Rationale: Only one chunk is ever displayed at once, buffering the full world
///            wastes memory (6x) and requires unnecessary clearing work each frame.
class Render {
private:
    /// @brief Viewport buffer - visual data for the currently selected chunk only
    /// Size: VIEWPORT_WIDTH x VIEWPORT_HEIGHT (not WORLD_SIZE)
    std::array<Cell, CHUNK_SIZE> _viewport{};

    /// @brief String buffer built each frame and flushed to stdout
    std::string _frame_buffer{};

public:
    /// @brief Constructor - initializes viewport and reserves frame buffer memory
    Render();

    /// @brief Destructor
    ~Render();

    /// @brief Render the currently selected chunk of the map
    ///
    /// Steps:
    /// 1. Clear the viewport buffer
    /// 2. Place each entity from the selected chunk into the viewport
    /// 3. Build the frame buffer string with ANSI color codes
    /// 4. Flush to stdout
    ///
    /// @param map The Map containing all chunks and the selected chunk index
    void drawWorld(const Map& map);
};

#endif //SPACE_STRATEGY_GAME_RENDER_H