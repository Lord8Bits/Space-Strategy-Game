#pragma once

#include <bitset>
#include <cstdlib>
#include "Constants.hpp"

/// @brief Tracks which world cells a player can see this turn and has ever explored.
///
/// Two independent bitsets indexed by flat world position (y * WORLD_WIDTH + x):
///   _visible    — cells in vision range RIGHT NOW (reset each turn)
///   _discovered — cells seen at least once (never reset — represents memory)
///
/// World index formula: world_x + world_y * (VIEWPORT_WIDTH * chunk_cols)
/// This matches Map's spatial layout so Perception and Map stay in sync.
///
/// Perception does NOT own any entities or ViewPort — it is pure data.
class Perception {
public:
    /// @brief Clear active visibility — call at the start of every turn.
    void resetVisibility() { _visible.reset(); }

    /// @brief Reveal the entire world — dev/demo tool only.
    void revealAll() { _visible.set(); _discovered.set(); }

    /// @brief Mark all cells within Manhattan distance `radius` of (cx, cy) as visible.
    /// @param cx      World X of the observing ship
    /// @param cy      World Y of the observing ship
    /// @param radius  Vision range (Manhattan distance)
    /// @param world_width  Total world width in cells (map.getChunkCols() * VIEWPORT_WIDTH)
    /// @param world_height Total world height in cells (map.getChunkRows() * VIEWPORT_HEIGHT)
    void updateVisibility(int cx, int cy, int radius, int world_width, int world_height) {
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                if (std::abs(dx) + std::abs(dy) > radius) continue;
                const int nx = cx + dx;
                const int ny = cy + dy;
                if (nx < 0 || ny < 0 || nx >= world_width || ny >= world_height) continue;
                const int index = nx + ny * world_width;
                if (index < 0 || index >= static_cast<int>(WORLD_SIZE)) continue;
                _visible[index]    = true;
                _discovered[index] = true;
            }
        }
    }

    /// @brief Returns true if (x, y) is currently visible this turn.
    bool isVisible(int x, int y, int world_width) const {
        const int index = x + y * world_width;
        if (index < 0 || index >= static_cast<int>(WORLD_SIZE)) return false;
        return _visible[index];
    }

    /// @brief Returns true if (x, y) has ever been explored.
    bool isDiscovered(int x, int y, int world_width) const {
        const int index = x + y * world_width;
        if (index < 0 || index >= static_cast<int>(WORLD_SIZE)) return false;
        return _discovered[index];
    }

    /// @brief Merge another Perception into this one (ally vision sharing).
    void mergeFrom(const Perception& other) {
        _visible    |= other._visible;
        _discovered |= other._discovered;
    }

    std::size_t visibleCount()    const { return _visible.count();    }
    std::size_t discoveredCount() const { return _discovered.count(); }

    /// @brief Fraction of the world explored so far (0.0 to 1.0).
    float explorationRatio() const {
        return static_cast<float>(_discovered.count()) / static_cast<float>(WORLD_SIZE);
    }

    const std::bitset<WORLD_SIZE>& getVisibleBitset()    const { return _visible;    }
    const std::bitset<WORLD_SIZE>& getDiscoveredBitset() const { return _discovered; }

private:
    std::bitset<WORLD_SIZE> _visible;    ///< Visible this turn only
    std::bitset<WORLD_SIZE> _discovered; ///< Ever seen — never reset
};