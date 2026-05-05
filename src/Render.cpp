//
// Created by lorend on 5/3/26.
//
#include <string>
#include "../include/Render.h"
#include "../include/Map.h"
#include <iostream>
#include <algorithm>
Render::Render()
{
    _world.fill({'.', White});
    _frame_buffer.reserve(MAX_BUFFER);
}

Render::~Render() = default;

void Render::draw(const Map& map)
{
    const Sector& sector = map._sectors[map._selected_sector];

    std::fill_n(_world.begin() + sector._x_start*sector._y_start, VIEWPORT_HEIGHT*VIEWPORT_WIDTH, Cell{'.', White});

    for (const SpaceEntity* entity : sector._entities) {
        if (entity == nullptr) continue;
        
        const int x{entity->_pos.x - sector._x_start};
        const int y{entity->_pos.y};
        const int idx{y * VIEWPORT_WIDTH + x};

        _world[idx] = entity->_cell;
    }

    const int start{sector._x_start * sector._y_start};
    const int end{sector._x_end * sector._y_end};

    Color last_color{_world[start].color};
    _frame_buffer = "\033[H";
    _frame_buffer += colorFormat(last_color);

    for (int i = start; i < end; i++) {
        if (last_color != _world[i].color){
            _frame_buffer += colorFormat(_world[i].color);
            last_color = _world[i].color;
        }
        _frame_buffer += _world[i].symbol;
    }

    _frame_buffer += "\x1b[0m";

    std::cout << _frame_buffer;
    std::cout.flush();
}

std::string_view Render::colorFormat(const Color color)
{
    switch (color) {
        case White :
            return "\x1b[0m";
        case Red :
            return "\x1b[31m";
        case Blue :
            return "\x1b[34m";
        case Green :
            return "\x1b[32m";
        default:
            return "";
    }
}

