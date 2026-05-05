#include "../../include/Entity.hpp"
#include "../Utils/Constants.hpp"
#include <iostream>

int Entity:: unique_id = 1;

Entity::Entity(const std::string& Name, const Position& Pos, Civilization* Owner)
    : id(unique_id++), name(Name), position(Pos), owner(Owner) {}

void Entity::render() const{
    if (owner)
        std::cout << owner -> getColorCode();
    std::cout << getSymbol() << GameUI::toAnsi(GameUI::CColor::RESET);
}