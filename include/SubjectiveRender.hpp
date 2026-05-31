#pragma once

#include "Render.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Perception.hpp"
#include "ViewPort.hpp"
#include "../src/Utils/Constants.hpp"
#include <array>
#include <string>

class Civilization;

/// @brief Visibility state of a world cell from one player's perspective.
enum class CellVisibility {
    Hidden,   ///< Never explored — render as blank fog
    Memory,   ///< Explored before, not visible now — render dimmed
    Visible   ///< Currently in vision range — render normally
};

/// @brief Classifies a world cell using a player's Perception.
inline CellVisibility classifyCell(const Perception& perc, int world_x, int world_y, int world_width) {
    if (perc.isVisible   (world_x, world_y, world_width)) return CellVisibility::Visible;
    if (perc.isDiscovered(world_x, world_y, world_width)) return CellVisibility::Memory;
    return CellVisibility::Hidden;
}

/// @brief Renders the game world filtered through a player's fog of war,
/// and draws the full status panel (resources, fleet, planets, commands).
class SubjectiveRender {
public:
    explicit SubjectiveRender(Render& render) : _render(render) {}

    /// @brief Draw the selected chunk filtered through the player's fog of war.
    void draw(const Player& player, const Map& map);
    void draw(const Player& player, const Map& map, const Civilization& civ);

    /// @brief Draw the complete game UI: world viewport + status panel + input prompt.
    /// @param player       The active player (fleet, FoW state)
    /// @param civ          The player's civilization (name, resources)
    /// @param map          The game world (sector navigation, entities)
    /// @param turn         Current turn number shown in the header
    /// @param last_message Last feedback line shown above the prompt
    void drawUI(const Player& player, const Civilization& civ,
                Map& map, int turn, const std::string& last_message);

    /// @brief Classify a world cell for a given player — useful for AI decisions.
    static CellVisibility classifyForPlayer(const Player& player, int world_x, int world_y, int world_width) {
        return classifyCell(player.getPerception(), world_x, world_y, world_width);
    }

private:
    void drawInternal(const Player& player, const Map& map, const Civilization* civ);

    Render& _render;
    std::array<GameUI::Cell, CHUNK_SIZE> _viewport{};
    std::string _frame_buffer;
};
