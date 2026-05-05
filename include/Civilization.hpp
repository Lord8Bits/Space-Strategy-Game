#pragma once
#include "../src/Utils/Enums.hpp"
#include "../src/Utils/Constants.hpp"

//Forward declaration & initialization
class Civilization{
protected:
    CivilizationType type;
    GameUI::CColor color;

public:
    Civilization(CivilizationType Type) : type(Type), color(draw(Type)) {}

    GameUI::CColor getColor() const { return color; }
    
    const char* getColorCode() const { return GameUI::toAnsi(color); }
};