//
// Created by lorend on 5/3/26.
//
#include <string>
#include "../../include/Render.h"
#include "../../include/Map.h"
#include "Utils/Constants.hpp"
#include "Utils/Position.hpp"

#include <iostream>
#include <algorithm>
Render::Render()
{
    _world.fill({'.', GameUI::Color::WHITE});
    _frame_buffer.reserve(MAX_BUFFER);
}

Render::~Render() = default;

void Render::drawWorld(const Map& map)
{
    const Chunk& chunk = map._chunks[map._selected_chunk];

    std::fill_n(_world.begin() + chunk._idx_start, chunk._idx_end, Cell{'.', GameUI::Color::WHITE});

    for (const auto& [entity_id, entity] : chunk._entities) {
        if (entity == nullptr) continue;

        const Vec2 pos{entity->getPosition()};
        const int idx{pos.y * VIEWPORT_WIDTH + pos.x + chunk._idx_start};

        _world[idx] = entity->getCell();
    }


    GameUI::Color last_color{GameUI::Color::WHITE};
    constexpr std::string_view return_code{"\033[H"};

    _frame_buffer = return_code;
    _frame_buffer += GameUI::toAnsi(last_color);

    for (int y = 0; y < VIEWPORT_HEIGHT; y++) {
        for (int x = 0; x < VIEWPORT_WIDTH; x++){
            const int idx{coordTranslation(x, y, map._selected_chunk)};

            if (last_color != _world[idx].color) {
                _frame_buffer += GameUI::toAnsi(_world[idx].color);
                last_color = _world[idx].color;
            }
            _frame_buffer += _world[idx].symbol;
        }
        _frame_buffer += '\n';
    }

    _frame_buffer += GameUI::toAnsi(GameUI::Color::RESET);

    std::cout << _frame_buffer;
    std::cout.flush();
}

int Render::coordTranslation(const int x, const int y, const int selected_sector)
{
    return (y * VIEWPORT_WIDTH + x) + selected_sector * (VIEWPORT_HEIGHT*VIEWPORT_WIDTH);
}
