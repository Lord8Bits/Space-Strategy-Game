#pragma once

#include "Render.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Perception.hpp"
#include "ViewPort.hpp"
#include "../src/Utils/Constants.hpp"
#include <array>
#include <string>
#include <iostream>

/// @brief Visibility state of a world cell from one player's perspective.
enum class CellVisibility {
    Hidden,   ///< Never explored — render as blank fog
    Memory,   ///< Explored before, not visible now — render dimmed
    Visible   ///< Currently in vision range — render normally
};

/// @brief Classifies a world cell using a player's Perception.
/// @param perc        The player's perception state
/// @param world_x     World X coordinate
/// @param world_y     World Y coordinate
/// @param world_width Total world width in cells
inline CellVisibility classifyCell(const Perception& perc, int world_x, int world_y, int world_width) {
    if (perc.isVisible   (world_x, world_y, world_width)) return CellVisibility::Visible;
    if (perc.isDiscovered(world_x, world_y, world_width)) return CellVisibility::Memory;
    return CellVisibility::Hidden;
}

/// @brief Renders the game world filtered through a player's fog of war.
///
/// Works by building its own viewport buffer — same approach as Render::drawWorld()
/// but each cell is classified before writing:
///   Visible  → normal entity symbol + civ color (or '.' if empty)
///   Memory   → '~' in dark grey (explored but not currently seen)
///   Hidden   → ' ' in black (never explored — blank)
///
/// Single flush per frame — no draw-then-overwrite race.
/// SubjectiveRender holds a non-owning reference to Render for makeCell().
class SubjectiveRender {
public:
    explicit SubjectiveRender(Render& render) : _render(render) {}

    /// @brief Draw the selected chunk filtered through the player's fog of war.
    /// @param player  The active player whose perception is used as a filter
    /// @param map     The full game world
    void draw(const Player& player, const Map& map) {
        const Chunk&   chunk       = map.getSelectedChunk();
        const int      world_width = map.getWorldWidth();
        const ViewPort vp(chunk.getXStart(), chunk.getYStart());
        const Perception& perc     = player.getPerception();

        // Step 1: Clear buffer with fog (Hidden = space, no color)
        _viewport.fill({' ', GameUI::Color::BLACK});

        // Step 2: Place visible entities into the buffer
        for (const int entity_id : chunk.getEntityIDs()) {
            const Entity* entity = map.getEntity(entity_id);
            if (!entity) continue;

            const Vec2 world_pos = entity->getPosition();
            if (!vp.isInViewport(world_pos.x, world_pos.y)) continue;

            const CellVisibility vis = classifyCell(perc, world_pos.x, world_pos.y, world_width);

            // Only draw the entity if it's currently visible
            if (vis != CellVisibility::Visible) continue;

            const int idx  = vp.toIndex(world_pos.x, world_pos.y);
            _viewport[idx] = Render::makeCell(*entity);
        }

        // Step 3: Apply fog/memory overlay to empty cells
        // (entity cells are already Visible — we only touch non-entity cells)
        for (int y = 0; y < VIEWPORT_HEIGHT; ++y) {
            for (int x = 0; x < VIEWPORT_WIDTH; ++x) {
                const int world_x = chunk.getXStart() + x;
                const int world_y = chunk.getYStart() + y;
                const int idx     = vp.toIndex(world_x, world_y);

                // Only apply fog to cells that weren't filled by an entity above
                if (_viewport[idx].symbol != ' ') continue;

                const CellVisibility vis = classifyCell(perc, world_x, world_y, world_width);
                switch (vis) {
                    case CellVisibility::Visible:
                        _viewport[idx] = {'.', GameUI::Color::WHITE};  // empty visible cell
                        break;
                    case CellVisibility::Memory:
                        _viewport[idx] = {'~', GameUI::Color::BLACK};  // dim memory
                        break;
                    case CellVisibility::Hidden:
                        _viewport[idx] = {' ', GameUI::Color::BLACK};  // total fog
                        break;
                }
            }
        }

        // Step 4: Build frame buffer and flush — identical to Render::drawWorld()
        GameUI::Color last_color = GameUI::Color::BLACK;
        _frame_buffer  = "\033[H";
        _frame_buffer += GameUI::toAnsi(last_color);

        for (int y = 0; y < VIEWPORT_HEIGHT; ++y) {
            for (int x = 0; x < VIEWPORT_WIDTH; ++x) {
                const int world_x = chunk.getXStart() + x;
                const int world_y = chunk.getYStart() + y;
                const int idx     = vp.toIndex(world_x, world_y);

                if (last_color != _viewport[idx].color) {
                    _frame_buffer += GameUI::toAnsi(_viewport[idx].color);
                    last_color = _viewport[idx].color;
                }
                _frame_buffer += _viewport[idx].symbol;
            }
            _frame_buffer += '\n';
        }

        _frame_buffer += GameUI::toAnsi(GameUI::Color::RESET);
        std::cout << _frame_buffer;
        std::cout.flush();
    }

    /// @brief Classify a world cell for a given player — useful for AI decisions.
    static CellVisibility classifyForPlayer(const Player& player, int world_x, int world_y, int world_width) {
        return classifyCell(player.getPerception(), world_x, world_y, world_width);
    }

private:
    Render& _render;
    std::array<GameUI::Cell, CHUNK_SIZE> _viewport{};
    std::string _frame_buffer;
};