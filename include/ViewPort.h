//
// Created by ilyasse-ch7 on 05/05/2026.
//

#ifndef SPACE_STRATEGY_GAME_VIEWPORT_H
#define SPACE_STRATEGY_GAME_VIEWPORT_H

#include "../src/Utils/Constants.hpp"
#include "../src/Utils/Position.hpp"


class ViewPort {
private :
    // World coordinate of the top-left cell visible in this viewport.
    int _offset_x;
    int _offset_y;

    // Size of the visible area.
    int _width{VIEWPORT_WIDTH};
    int _height{VIEWPORT_HEIGHT};

public:
    ViewPort(int offset_x, int offset_y);

    // Convert local viewport coordinates to world coordinates.
    // Example: local (2, 3) with offset (40, 20) returns world (42, 23).
    Vec2 toWorldPosition(int local_x, int local_y) const;

    // Convert world coordinates to local viewport coordinates.
    // Example: world (42, 23) with offset (40, 20) returns local (2, 3).
    Vec2 toLocalPosition(int world_x, int world_y) const;

    // Convert world coordinates to a local one-dimensional viewport index.
    // Formula: local_x + local_y * width.
    int toIndex(int world_x, int world_y) const;

    // Convert world coordinates to an index in a chunked world buffer.
    // This matches the render formula:
    // local_index + selected_chunk * (width * height).
    int toBufferIndex(int world_x, int world_y, int selected_chunk) const;

    // Check if local viewport coordinates are within bounds.
    // Returns true if (local_x, local_y) is valid on the visible screen.
    bool isInBounds(int local_x, int local_y) const;

    // Check if world coordinates are currently visible in the viewport.
    // Returns true if (world_x, world_y) is inside the active viewing area.
    bool isInViewport(int world_x, int world_y) const;
};


#endif //SPACE_STRATEGY_GAME_VIEWPORT_H
