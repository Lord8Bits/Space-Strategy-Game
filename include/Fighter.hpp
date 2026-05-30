#pragma once
#include "Ship.hpp"
#include "../src/Utils/Constants.hpp"

class Fighter : public Ship{
    int _agility;

public:
    Fighter(const std::string& Name, const Vec2& Pos, Civilization* Owner);

    bool tryDodge(const Ship& attacker) const override;
    bool canCounterAttack() const override { return true; }
    int  calculateDamageAgainst(const Entity& target) const override;

    std::string getDetailedInfo() const override;
    void levelUp() override;

    int getAgility() const { return _agility; }
    ShipType getShipType() const override { return ShipType::FIGHTER; }
    std::string getSymbol() const override { return "▲"; }
    int getXpReward() const override { return GameConstants::FIGHTER_XP_REWARD; }
};