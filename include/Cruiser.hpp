#pragma once
#include "Ship.hpp"
#include "../src/Utils/Constants.hpp"

class Cruiser : public Ship{
    int _shieldStrength;

public:
    Cruiser(const std::string& Name, const Vec2& Pos, Civilization* Owner);

    int absorbDamage(int damage) override;

    std::string getDetailedInfo() const override;
    void levelUp() override;

    int getShieldStrength() const { return _shieldStrength; }
    ShipType getShipType() const override { return ShipType::CRUISER; }
    std::string getSymbol() const override { return "◆"; }  // ◆
    int getXpReward() const override { return GameConstants::CRUISER_XP_REWARD; }
};