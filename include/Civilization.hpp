#pragma once
#include "Updatable.hpp"
#include "../src/Utils/Enums.hpp"
#include "../src/Utils/Constants.hpp"

//Forward declaration & initialization
class Civilization : public Updatable{
protected:
    CivilizationType type;
    GameUI::Color color;

public:
    Civilization(CivilizationType Type) : type(Type), color(civToColor(Type)) {}

    void update() override;

    GameUI::Color getColor() const { return color; }
    
    const char* getColorCode() const { return GameUI::toAnsi(color); }
};