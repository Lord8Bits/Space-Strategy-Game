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

void Render::drawWorld(const Map& map)
{
    const Sector& sector = map._sectors[map._selected_sector];

    std::fill_n(_world.begin() + sector._y_start * VIEWPORT_WIDTH + sector._x_start, VIEWPORT_HEIGHT*VIEWPORT_WIDTH, Cell{'.', White});

    for (const auto& entity : sector._entities) {
        if (entity == nullptr) continue;
        
        const int x{entity->_pos.x - sector._x_start};
        const int y{entity->_pos.y};
        const int idx{y * VIEWPORT_WIDTH + x};

        _world[idx] = entity->_cell;
    }


    Color last_color{White};
    constexpr std::string_view return_code{"\033[H"};

    _frame_buffer =  return_code;
    _frame_buffer += colorFormat(last_color);

    for (int y = 0; y < VIEWPORT_HEIGHT; y++) {
        for (int x = 0; x < VIEWPORT_WIDTH; x++){
            const int idx{coordTranslation(x, y, map._selected_sector)};

            if (last_color != _world[idx].color) {
                _frame_buffer += colorFormat(_world[idx].color);
                last_color = _world[idx].color;
            }
            _frame_buffer += _world[idx].symbol;
        }
        _frame_buffer += '\n';
    }

    _frame_buffer += colorFormat(White);

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

int Render::coordTranslation(const int x, const int y, const int selected_sector)
{
    return (y * VIEWPORT_WIDTH + x) + selected_sector * (VIEWPORT_HEIGHT*VIEWPORT_WIDTH);
}
