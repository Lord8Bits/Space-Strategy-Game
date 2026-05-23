#pragma once
#include "Enums.hpp"

static constexpr int VIEWPORT_WIDTH = 80;
static constexpr int VIEWPORT_HEIGHT = 20;
static constexpr int CHUNK_COUNT = 6;
static constexpr int WORLD_SIZE = VIEWPORT_HEIGHT * VIEWPORT_WIDTH * CHUNK_COUNT;
static constexpr int MAX_BUFFER = VIEWPORT_HEIGHT * VIEWPORT_WIDTH * 3;

namespace GameUI{
    enum class Color{
        CYAN,
        RED,
        BLACK,
        WHITE,
        YELLOW,
        GREEN,
        RESET
    };

    inline constexpr const char* toAnsi(Color color){
        switch(color){
            case Color::CYAN : return "\x1b[36m";
            case Color::RED : return "\x1b[31m";
            case Color::BLACK : return "\x1b[90m";
            case Color::WHITE : return "\x1b[37m";
            case Color::YELLOW : return "\x1b[33m";
            case Color::GREEN : return "\x1b[32m";
            case Color::RESET : return "\x1b[0m";
            default : return "\x1b[0m";
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
