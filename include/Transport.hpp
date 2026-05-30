#pragma once
#include "Ship.hpp"
#include "../src/Utils/Constants.hpp"
#include "Resource.hpp"

class Transport : public Ship{
    int _cargoCapacity;
    Resource _cargo;

public:
    Transport(const std::string& Name, const Vec2& Pos, Civilization* Owner);

    std::string getDetailedInfo() const override;
    CombatResult attack(Entity& target, CombatSystem& combatSystem) override;
    void levelUp() override;
    void interactEntity(Entity* other, CombatSystem& combatSystem) override;

    bool loadCargo(const Resource& resource);
    Resource unloadCargo(const Resource& resource);
    bool isFull() const { return _cargo.total() >= _cargoCapacity; }
    bool isEmpty() const { return _cargo.total() <= 0; }    

    ShipType getShipType() const override { return ShipType::TRANSPORT; }
    char getSymbol() const override { return 'T'; }
    int getXpReward() const override { return GameConstants::TRANSPORT_XP_REWARD; }
    Resource getCargo() const { return _cargo; }
    int getCargoCapacity() const { return _cargoCapacity; }
};