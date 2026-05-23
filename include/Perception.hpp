#pragma once

#include <bitset>
#include <cstdlib>
#include "Utils/Constants.hpp"


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
                if (nx < 0 || ny < 0 || nx >= VIEWPORT_WIDTH || ny >= VIEWPORT_HEIGHT)
                    continue;
                std::size_t idx = ny * VIEWPORT_WIDTH + nx;
                visible_[idx]    = true;
                discovered_[idx] = true;
            }
        }
    }

    /// @brief Returns true if the cell is visible this turn
    bool isVisible(int x, int y) const {
        return inBounds(x, y) && visible_[toIndex(x, y)];
    }

    /// @brief Returns true if the cell has been explored at least once
    bool isDiscovered(int x, int y) const {
        return inBounds(x, y) && discovered_[toIndex(x, y)];
    }

    /// @brief Returns the number of cells currently visible this turn
    std::size_t visibleCount() const { return visible_.count(); }

    /// @brief Returns the total number of cells discovered so far
    std::size_t discoveredCount() const { return discovered_.count(); }

    /// @brief Returns the percentage of the map explored (0.0 to 1.0)
    float explorationRatio() const {
        return static_cast<float>(discovered_.count())
             / static_cast<float>(WORLD_SIZE);
    }

    /// @brief Merges another player's perception into this one (ally vision sharing)
    void mergeFrom(const Perception& other) {
        visible_    |= other.visible_;
        discovered_ |= other.discovered_;
    }

    const std::bitset<WORLD_SIZE>& getVisibleBitset()    const { return visible_; }
    const std::bitset<WORLD_SIZE>& getDiscoveredBitset() const { return discovered_; }

private:

    /// Cells visible this turn only — reset every turn
    std::bitset<WORLD_SIZE> _visible;

    /// Cells seen at least once — never reset
    std::bitset<WORLD_SIZE> _discovered;

    static std::size_t toIndex(int x, int y) {
        return static_cast<std::size_t>(y) * VIEWPORT_WIDTH + x;
    }

    static bool inBounds(int x, int y) {
        return x >= 0 && y >= 0 && x < VIEWPORT_WIDTH && y < VIEWPORT_HEIGHT;
    }
};
