#ifndef SPACE_STRATEGY_GAME_RENDER_H
#define SPACE_STRATEGY_GAME_RENDER_H

#include <array>
#include <string>
#include "Map.h"
#include "ViewPort.h"
#include "../src/Utils/Constants.hpp"

/// @brief Handles all rendering of the game world to the terminal
///
/// Render is responsible for VISUAL concerns only:
///   - Building GameUI::Cell from Entity's public interface (symbol + civ color)
///   - Maintaining a viewport buffer for the selected chunk
///   - Flushing ANSI-colored output to terminal
///
/// Render does NOT know about gameplay (no Cell inside Entity)
class Render {
private:
    /// Viewport buffer - one chunk at a time (CHUNK_SIZE cells)
    std::array<GameUI::Cell, CHUNK_SIZE> _viewport{};

    /// String buffer built each frame and flushed to stdout
    std::string _frame_buffer{};

public:
    Render();
    ~Render();

    /// @brief Render the currently selected chunk to the terminal
    void drawWorld(const Map& map);

    /// @brief Build a GameUI::Cell from an Entity's public interface
    /// Uses entity.getSymbol() and entity.getCivOwner()->getColor()
    /// @param entity The entity to build a cell for
    /// @return GameUI::Cell with symbol and color
    static GameUI::Cell makeCell(const Entity& entity);
};

#endif //SPACE_STRATEGY_GAME_RENDER_H