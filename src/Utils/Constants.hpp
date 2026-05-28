#pragma once
#include "Enums.hpp"
#include <string>

static constexpr int VIEWPORT_WIDTH  = 80;
static constexpr int VIEWPORT_HEIGHT = 20;
static constexpr int CHUNK_SIZE      = VIEWPORT_WIDTH * VIEWPORT_HEIGHT;
static constexpr int CHUNK_COUNT     = 6;
static constexpr int MAX_BUFFER      = CHUNK_SIZE * 3;
static constexpr int WORLD_SIZE      = CHUNK_SIZE * CHUNK_COUNT;

namespace GameUI {

    enum class Color {
        CYAN, RED, BLACK, WHITE, YELLOW, GREEN, RESET
    };

    inline constexpr std::string_view toAnsi(const Color color) {
        switch (color) {
            case Color::CYAN   : return "\x1b[36m";
            case Color::RED    : return "\x1b[31m";
            case Color::BLACK  : return "\x1b[90m";
            case Color::WHITE  : return "\x1b[37m";
            case Color::YELLOW : return "\x1b[33m";
            case Color::GREEN  : return "\x1b[32m";
            case Color::RESET  : return "\x1b[0m";
            default            : return "";
        }
    }

    /// @brief Rendering cell - belongs to the rendering layer, not gameplay
    /// Built by Render::makeCell() from Entity's public interface
    struct Cell {
        char symbol{'.'};
        Color color{Color::WHITE};
    };

}  // namespace GameUI

constexpr GameUI::Color civToColor(CivilizationType type) {
    switch (type) {
        case CivilizationType::PLAYER      : return GameUI::Color::CYAN;
        case CivilizationType::AGGRESSIVE  : return GameUI::Color::RED;
        case CivilizationType::NEUTRAL     : return GameUI::Color::BLACK;
        case CivilizationType::EXPANSIONIST: return GameUI::Color::YELLOW;
        case CivilizationType::PEACEFUL    : return GameUI::Color::GREEN;
        default                            : return GameUI::Color::RESET;
    }

}

namespace GameConstants {
    //Fighter Constants
    constexpr int FIGHTER_MAX_HEALTH = 100;
    constexpr int FIGHTER_ATTACK = 25;
    constexpr int FIGHTER_MOVEMENT_RANGE = 4;
    constexpr int FIGHTER_VISION_RANGE = 4;
    constexpr int FIGHTER_AGILITY = 40;
    
    //Cruiser Constants
    constexpr int CRUISER_MAX_HEALTH = 300;
    constexpr int CRUISER_ATTACK = 50;
    constexpr int CRUISER_MOVEMENT_RANGE = 2;
    constexpr int CRUISER_VISION_RANGE = 4;
    constexpr int CRUISER_SHIELD = 50;
    
    //Transport Constants
    constexpr int TRANSPORT_MAX_HEALTH = 150;
    constexpr int TRANSPORT_ATTACK = 0;
    constexpr int TRANSPORT_MOVEMENT_RANGE = 3;
    constexpr int TRANSPORT_VISION_RANGE = 3;
    constexpr int TRANSPORT_CARGO = 1000;

    //Xp Reward Constants
    constexpr int FIGHTER_XP_REWARD = 40;
    constexpr int CRUISER_XP_REWARD = 80;
    constexpr int TRANSPORT_XP_REWARD = 30;
}