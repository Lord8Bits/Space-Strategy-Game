#include <string>
#include "../../include/Render.h"
#include "../../include/Map.h"
#include "../../include/ViewPort.h"
#include <iostream>

Render::Render()
{
    _viewport.fill({'.', GameUI::Color::WHITE});
    _frame_buffer.reserve(MAX_BUFFER);
}

Render::~Render() = default;

void Render::drawWorld(const Map& map)
{
    const Chunk& chunk = map._chunks[map._selected_chunk];

    // Build a ViewPort centered on the selected chunk's world origin
    // offset_x/y = top-left world coordinate of this chunk
    const ViewPort vp(chunk._x_start, chunk._y_start);

    // Step 1: Clear the viewport buffer
    _viewport.fill({'.', GameUI::Color::WHITE});

    // Step 2: Place each entity into the viewport buffer
    // ViewPort handles world → local conversion and bounds checking
    for (const auto& [entity_id, entity] : chunk._entities) {
        if (!entity) continue;

        const Vec2 world_pos = entity->getPosition();

        // Skip entity if outside the visible viewport
        if (!vp.isInViewport(world_pos.x, world_pos.y)) continue;

        // Convert world position to flat buffer index using ViewPort
        const int idx = vp.toIndex(world_pos.x, world_pos.y);
        _viewport[idx] = entity->getCell();
    }

    // Step 3: Build the frame buffer with ANSI color codes
    GameUI::Color last_color = GameUI::Color::WHITE;
    _frame_buffer  = "\033[H";                         // Move cursor to top-left
    _frame_buffer += GameUI::toAnsi(last_color);

    for (int y = 0; y < VIEWPORT_HEIGHT; y++) {
        for (int x = 0; x < VIEWPORT_WIDTH; x++) {
            const int idx = y * VIEWPORT_WIDTH + x;

            // Only emit a color code when color changes (reduces output size)
            if (last_color != _viewport[idx].color) {
                _frame_buffer += GameUI::toAnsi(_viewport[idx].color);
                last_color = _viewport[idx].color;
            }
            _frame_buffer += _viewport[idx].symbol;
        }
        _frame_buffer += '\n';
    }

    _frame_buffer += GameUI::toAnsi(GameUI::Color::RESET);

    // Step 4: Flush to terminal
    std::cout << _frame_buffer;
    std::cout.flush();
}