//
// Created by ilyasse-ch7 on 05/05/2026.
//

#ifndef SPACE_STRATEGY_GAME_VIEWPORT_H
#define SPACE_STRATEGY_GAME_VIEWPORT_H
#include <utility>

#include "Utils/Constants.hpp"
#include "Utils/Position.hpp"

/// @brief Maps between world coordinates and screen/local coordinates
///
/// The ViewPort represents a "window" into the game world.
/// It knows where in the world it is positioned (offset_x, offset_y)
/// and provides conversions between:
///   - World coordinates (absolute position in game world)
///   - Local coordinates (position on screen, 0-based)
///   - Buffer indices (flat array index for rendering)
class ViewPort {
private:
    /// World coordinate of the top-left cell visible in this viewport
    int _offset_x;
    int _offset_y;

    /// Size of the visible area
    int _width{VIEWPORT_WIDTH};
    int _height{VIEWPORT_HEIGHT};

public:
    /// @brief Constructor
    /// @param offset_x World X coordinate of the top-left visible cell
    /// @param offset_y World Y coordinate of the top-left visible cell
    /// @throws std::invalid_argument if offsets are negative
    ViewPort(int offset_x, int offset_y);

    /// @brief Convert local screen coordinates to world coordinates
    /// @example local (2, 3) with offset (40, 20) → world (42, 23)
    /// @throws std::out_of_range if local coordinates are outside viewport
    std::pair<int, int> toWorldPosition(int local_x, int local_y) const;

    /// @brief Convert world coordinates to local screen coordinates
    /// @example world (42, 23) with offset (40, 20) → local (2, 3)
    /// @throws std::out_of_range if world coordinates are outside viewport
    Vec2 toLocalPosition(int world_x, int world_y) const;

    /// @brief Convert world coordinates to a flat viewport buffer index
    /// @details Formula: local_x + local_y * width
    /// @throws std::out_of_range if world coordinates are outside viewport
    int toIndex(int world_x, int world_y) const;

    /// @brief Convert world coordinates to an index in the full world buffer
    /// @details Formula: toIndex() + selected_chunk * (width * height)
    /// @throws std::out_of_range if world coordinates or chunk index are invalid
    int toBufferIndex(int world_x, int world_y, int selected_chunk) const;

    /// @brief Check if local screen coordinates are within the visible area
    /// @return true if (local_x, local_y) is a valid screen position
    bool isInBounds(int local_x, int local_y) const;

    /// @brief Check if world coordinates fall within this viewport's visible area
    /// @return true if (world_x, world_y) is currently visible on screen
    bool isInViewport(int world_x, int world_y) const;
};

#endif //SPACE_STRATEGY_GAME_VIEWPORT_H