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
Cell Render::makeCell(const Entity& entity) const {
    GameUI::Color color = GameUI::Color::WHITE;

    if (entity.getCivOwner()) {
        color = entity.getCivOwner()->getColor();
    }

    return Cell{entity.getSymbol(), color};
}
void Render::drawWorld(const Map& map)
{
    const Chunk& chunk = map.getSelectedChunk();

    // Build ViewPort using the chunk's world origin as offset
    const ViewPort vp(chunk.getXStart(), chunk.getYStart());

    // Step 1: Clear the viewport buffer
    _viewport.fill({'.', GameUI::Color::WHITE});

    // Step 2: Place each visible entity into the viewport buffer
    for (const auto& [entity_id, entity] : chunk.getEntities()) {
        if (!entity) continue;

        const Vec2 world_pos = entity->getPosition();

        // Skip if outside this chunk's visible area
        if (!vp.isInViewport(world_pos.x, world_pos.y)) continue;

        // Convert world position to flat buffer index
        const int idx = vp.toIndex(world_pos.x, world_pos.y);
        _viewport[idx] = entity->getCell();
    }

    // Step 3: Build frame buffer with ANSI color codes
    GameUI::Color last_color = GameUI::Color::WHITE;
    _frame_buffer  = "\033[H";
    _frame_buffer += GameUI::toAnsi(last_color);

    for (int y = 0; y < VIEWPORT_HEIGHT; y++) {
        for (int x = 0; x < VIEWPORT_WIDTH; x++) {
            const int idx = y * VIEWPORT_WIDTH + x;

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