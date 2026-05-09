#pragma once
#include "Enums.hpp"
#include <string>

static constexpr int VIEWPORT_WIDTH = 80;                                                   ///< Width of visible viewport
static constexpr int VIEWPORT_HEIGHT = 20;                                                 ///< Height of visible viewport
static constexpr int SECTOR_COUNT = 6;                                                    ///< Number of Sectors
static constexpr int WORLD_SIZE = VIEWPORT_HEIGHT*VIEWPORT_WIDTH * SECTOR_COUNT;         ///< Total size of the world buffer
static constexpr int MAX_BUFFER = WORLD_SIZE * 3;                                       ///< Maximum buffer size

namespace GameUI{
    /// @brief Color enumeration for terminal color output
    enum class Color{
        CYAN,
        RED,
        BLACK,
        WHITE,
        YELLOW,
        GREEN,
        RESET
    };
    /// @brief Convert a color enum to ANSI terminal color code
    /// @param color The Color enum value
    /// @return ANSI escape sequence string for the color
    inline constexpr std::string_view toAnsi(const Color color){
        switch(color){
            case Color::CYAN : return "\x1b[36m";
            case Color::RED : return "\x1b[31m";
            case Color::BLACK : return "\x1b[90m";
            case Color::WHITE : return "\x1b[37m";
            case Color::YELLOW : return "\x1b[33m";
            case Color::GREEN : return "\x1b[32m";
            case Color::RESET : return "\x1b[0m";
            default : return "";
        }
    }
}

constexpr GameUI::Color civToColor(CivilizationType type){
    switch(type){
        case CivilizationType::PLAYER : return GameUI::Color::CYAN;
        case CivilizationType::AGGRESSIVE : return GameUI::Color::RED;
        case CivilizationType::NEUTRAL : return GameUI::Color::BLACK;
        case CivilizationType::EXPANSIONIST : return GameUI::Color::YELLOW;
        case CivilizationType::PEACEFUL : return GameUI::Color::GREEN;
        default : return GameUI::Color::RESET;
    }
}