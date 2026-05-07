#pragma once
#include "../include/Enums.h"

struct CivilizationType;
//Forward declaration & initialization
class Civilization{
protected:
    CivilizationType type;
    GameUI::Color color;

public:
    Civilization(CivilizationType Type) : type(Type), color(draw(Type)) {}

    GameUI::Color getColor() const { return color; }
    
    std::string_view getColorCode() const { return GameUI::toAnsi(color); }
};