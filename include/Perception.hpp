#pragma once

#include <bitset>
#include <cstdlib>
#include "Utils/Constants.hpp"
#include <ViewPort.h>


class Perception {
public:

    /// @brief Resets visibility for the current turn.
    ///        Called at the start of each turn before updating ships.
    ///        discovered_ is never reset — it represents long-term memory.
    void resetVisibility() {
        visible_.reset();
    }

    /// @brief Updates visibility from a position (cx, cy) with a given radius.
    ///        Uses Manhattan distance. Marks cells as both visible and discovered.
    /// @param cx, cy  Center position (ship coordinates)
    /// @param radius  Vision range of the ship
    void updateVisibility(int cx, int cy, int radius) {
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                if (std::abs(dx) + std::abs(dy) > radius) continue;
                int nx = cx + dx;
                int ny = cy + dy;
                if (!_viewport.isInBounds(nx, ny)) continue;
                int index = _viewport.toIndex(nx, ny);
                _visible[index]    = true;
                _discovered[index] = true;
            }
        }
    }

    /// @brief Returns true if the cell is visible this turn
    bool isVisible(int x, int y) const {
        if (!_viewport.isInBounds(x, y)) return false;
        return _visible[_viewport.toIndex(x, y)];
    }

    /// @brief Returns true if the cell has been explored at least once
    bool isDiscovered(int x, int y) const {
        if (!_viewport.isInBounds(x, y)) return false;
        return _discovered[_viewport.toIndex(x, y)];
    }

    /// @brief Returns the number of cells currently visible this turn
    std::size_t visibleCount() const { return _visible.count(); }

    /// @brief Returns the total number of cells discovered so far
    std::size_t discoveredCount() const { return _discovered.count(); }

    /// @brief Returns the percentage of the map explored (0.0 to 1.0)
    float explorationRatio() const {
        return static_cast<float>(_discovered.count())
             / static_cast<float>(WORLD_SIZE);
    }

    /// @brief Merges another player's perception into this one (ally vision sharing)
    void mergeFrom(const Perception& other) {
        _visible    |= other._visible;
        _discovered |= other._discovered;
    }

    const std::bitset<WORLD_SIZE>& getVisibleBitset()    const { return _visible; }
    const std::bitset<WORLD_SIZE>& getDiscoveredBitset() const { return _discovered; }

private:
    
    ViewPort _viewport;
    /// Cells visible this turn only — reset every turn
    std::bitset<WORLD_SIZE> _visible;

    /// Cells seen at least once — never reset
    std::bitset<WORLD_SIZE> _discovered;

};
