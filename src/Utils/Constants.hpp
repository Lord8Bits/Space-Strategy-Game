#pragma once
#include "Enums.hpp"

namespace GameUI{
    enum class CColor{
        CYAN,
        RED,
        BLACK,
        WHITE,
        YELLOW,
        GREEN,
        RESET
    };

    inline constexpr const char* toAnsi(CColor color){
        switch(color){
            case CColor::CYAN : return "\x1b[36m";
            case CColor::RED : return "\x1b[31m";
            case CColor::BLACK : return "\x1b[90m";
            case CColor::WHITE : return "\x1b[37m";
            case CColor::YELLOW : return "\x1b[33m";
            case CColor::GREEN : return "\x1b[32m";
            case CColor::RESET : return "\x1b[0m";
            default : return "\x1b[0m";
        }
    }
}

constexpr GameUI::CColor draw(CivilizationType type){
    switch(type){
        case CivilizationType::PLAYER : return GameUI::CColor::CYAN;
        case CivilizationType::AGGRESSIVE : return GameUI::CColor::RED;
        case CivilizationType::NEUTRAL : return GameUI::CColor::BLACK;
        case CivilizationType::EXPANSIONIST : return GameUI::CColor::YELLOW;
        case CivilizationType::PEACEFUL : return GameUI::CColor::GREEN;
        default : return GameUI::CColor::RESET;
    }
}