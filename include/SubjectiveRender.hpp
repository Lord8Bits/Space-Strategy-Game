#pragma once

#include "Render.h"
#include "Map.h"
#include "Player.hpp"
#include "Perception.hpp"

/// @brief Represents the visibility state of a cell from the active player's point of view
enum class Visibility {
    Hidden,    /// Never explored — do not render
    Memory,    /// Explored before but not visible this turn — render dimmed
    Visible    /// Currently visible this turn — render normally
};

/// @brief Classifies a single cell based on the player's perception bitsets
/// @param perc  The player's Perception object
/// @param x, y  Grid coordinates of the cell
/// @return   The Visibility state of that cell
inline Visibility classifyCell(const Perception& perc, int x, int y) {
    if (perc.isVisible(x, y)) return Visibility::Visible;
    if (perc.isDiscovered(x, y)) return Visibility::Memory;
    return Visibility::Hidden;
}

/// @brief Filters world entities based on the active player's fog of war
///        before passing them to the Render system
class SubjectiveRender {
public:

    /// @brief Constructor
    /// @param render  Reference to the Render instance owned by GameEngine
    explicit SubjectiveRender(Render& render)
        : render_(render)
    {}

    /// @brief Main draw call — renders the world from the active player's point of view
    ///        Draws the full world first, then overlays the fog of war on top
    /// @param player  The active player whose perception is used as a filter
    /// @param map   The full world map containing all chunks and entities
    void draw(const Player& player, Map& map) {
        render_.drawWorld(map);
        applyFogOverlay(player.getPerception());
    }

    /// @brief Classifies a cell from a given player's point of view
    ///        Useful for AI decisions and debug tools
    /// @param player  The player whose perception is used
    /// @param x, y    Grid coordinates to classify
    /// @return   The Visibility state of that cell
    static Visibility classifyForPlayer(const Player& player, int x, int y) {
        return classifyCell(player.getPerception(), x, y);
    }

private:

    Render& render_;

    /// @brief Overlays fog of war symbols on top of the already-rendered frame
    ///        Hidden cells are replaced by a dark grey dot
    ///        Memory cells are replaced by a dimmed tilde
    ///        Visible cells are left untouched
    /// @param perc  The active player's Perception object
    void applyFogOverlay(const Perception& perc) const {

        // Grid dimensions used to iterate over every cell of the viewport
        constexpr int W = static_cast<int>(WORLD_W);
        constexpr int H = static_cast<int>(WORLD_H);

        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {

                // Determine the visibility state of this cell
                // from the active player's perception bitsets
                Visibility vis = classifyCell(perc, x, y);

                if (vis == Visibility::Hidden) {
                    // Move the terminal cursor to (x, y) then draw opaque fog
                    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H"
                              << "\033[90m.\033[0m";
                }
                else if (vis == Visibility::Memory) {
                    // Cell was explored before but is no longer in vision range.
                    // Move cursor then draw a dimmed tilde to indicate a memorized area
                    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H"
                              << "\033[2m~\033[0m";
                }
                // Visible cells are left untouched — Render already drew them correctly
            }
        }

        // Flush stdout to make sure the full frame appears at once
        std::cout.flush();
    }
};
