//
// Created by ilyasse-ch7 on 05/05/2026.
//

#include "ViewPort.h"
#include <stdexcept>
#include "Utils/Position.hpp"

ViewPort::ViewPort(const int offset_x, const int offset_y)
{
    if (offset_x < 0 || offset_y < 0) {
        throw std::invalid_argument("Viewport offset must be non-negative");
    }
    _offset_x = offset_x;
    _offset_y = offset_y;
}

std::pair<int, int> ViewPort::toWorldPosition(const int local_x, const int local_y) const
{
    if (!isInBounds(local_x, local_y)) {
        throw std::out_of_range("Local coordinates are outside the viewport.");
    }
    return {_offset_x + local_x, _offset_y + local_y};
}

Vec2 ViewPort::toLocalPosition(const int world_x, const int world_y) const
{
    if (!isInViewport(world_x, world_y)) {
        throw std::out_of_range("World coordinates are outside the viewport.");
    }
    return Vec2{world_x - _offset_x, world_y - _offset_y};
}

int ViewPort::toIndex(const int world_x, const int world_y) const
{
    const auto [local_x, local_y] = toLocalPosition(world_x, world_y);
    return local_x + local_y * _width;
}

int ViewPort::toBufferIndex(const int world_x, const int world_y, const int selected_chunk) const
{
    if (selected_chunk < 0) {
        throw std::out_of_range("Selected chunk must be non-negative.");
    }
    return toIndex(world_x, world_y) + selected_chunk * _width * _height;
}

bool ViewPort::isInBounds(const int local_x, const int local_y) const
{
    return local_x >= 0 && local_y >= 0
        && local_x < _width && local_y < _height;
}

bool ViewPort::isInViewport(const int world_x, const int world_y) const
{
    return world_x >= _offset_x && world_x < _offset_x + _width
        && world_y >= _offset_y && world_y < _offset_y + _height;
}