#pragma once
#include "Updatable.hpp"
#include "Utils/Enums.hpp"
#include "Utils/Constants.hpp"

//Forward declaration & initialization
class Civilization : public Updatable{
protected:
    CivilizationType type;
    GameUI::Color color;

public:
    Civilization(CivilizationType Type) : type(Type), color(civToColor(Type)) {}

    void update() override;

    GameUI::Color getColor() const { return color; }
};