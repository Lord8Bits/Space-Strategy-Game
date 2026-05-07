//
// Created by lorend on 5/7/26.
//

#ifndef SPACE_STRATEGY_GAME_ENUMS_H

#include <string>

#define SPACE_STRATEGY_GAME_ENUMS_H

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
#endif //SPACE_STRATEGY_GAME_ENUMS_H
