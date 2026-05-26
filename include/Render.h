#ifndef SPACE_STRATEGY_GAME_RENDER_H
#define SPACE_STRATEGY_GAME_RENDER_H
#include <array>
#include "Entity.hpp"
#include "Map.h"
#include "ViewPort.h"
struct Cell {
    char symbol;
    GameUI::Color color;
};
/// @brief Handles all rendering of the game world to the terminal
///
/// Design decision: _viewport only stores one chunk at a time (CHUNK_SIZE cells)
/// Rationale: Only one chunk is ever displayed at once, so buffering the full
///            world wastes 6x memory and requires unnecessary clearing work.
class Render {
private:
    /// Viewport buffer - visual data for the currently selected chunk only
    std::array<Cell, CHUNK_SIZE> _viewport{};

    /// String buffer built each frame and flushed to stdout
    std::string _frame_buffer{};

public:
    Render();
    ~Render();

    /// @brief Render the currently selected chunk of the map to the terminal
    void drawWorld(const Map& map);
};

#endif //SPACE_STRATEGY_GAME_RENDER_H