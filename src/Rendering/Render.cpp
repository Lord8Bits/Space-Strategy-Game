#include <string>
#include "../../include/Render.hpp"
#include "../../include/Map.hpp"
#include "../../include/ViewPort.hpp"
#include <iostream>

Render::Render()
{
    _viewport.fill({'.',  GameUI::Color::WHITE});
    _frame_buffer.reserve(MAX_BUFFER);
}

Render::~Render() = default;

GameUI::Cell Render::makeCell(const Entity& entity)
{
    // Color comes from the owning civilization
    // If no owner, default to WHITE (neutral/unclaimed)
    const GameUI::Color color = entity.getCivOwner()
        ? entity.getCivOwner()->getColor()
        : GameUI::Color::WHITE;

    return GameUI::Cell{entity.getSymbol(), color};
}

void Render::drawWorld(const Map& map)
{
    const Chunk&   chunk = map.getSelectedChunk();
    const ViewPort vp(chunk.getXStart(), chunk.getYStart());

    // Step 1: Clear the viewport buffer
    _viewport.fill({'.', GameUI::Color::WHITE});

    // Step 2: Iterate entity IDs, fetch from Map, build Cell for rendering
    for (const int entity_id : chunk.getEntityIDs()) {
        const Entity* entity = map.getEntity(entity_id);
        if (!entity) continue;

        const Vec2 world_pos = entity->getPosition();
        if (!vp.isInViewport(world_pos.x, world_pos.y)) continue;

        const int idx      = vp.toIndex(world_pos.x, world_pos.y);
        _viewport[idx]     = makeCell(*entity);  // Build Cell here, not in Entity
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