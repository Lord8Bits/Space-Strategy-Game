#pragma once
#include "Ship.hpp"
#include "../src/Utils/Constants.hpp"

// Forward declaration
struct Resources {
    int energy;
    int ore;
    int food;
    
    Resources(int e = 0, int o = 0, int f = 0) 
        : energy(e), ore(o), food(f) {}
    
    int total() const { return energy + ore + food; }

    friend bool operator>(const Resources& cargo, const Resources& resources){
        return cargo.energy > resources.energy 
            || cargo.ore > resources.ore
            || cargo.food > resources.food;
    }
};


class Transport : public Ship{
    int _cargoCapacity;
    Resources _cargo;

public:
    Transport(const std::string& Name, const Vec2& Pos, Civilization* Owner);

    std::string getDetailedInfo() const override;
    void attack(Entity& target, CombatSystem& CombatSystem) override;
    void levelUp() override;
    void interactEntity(Entity* other, CombatSystem& combatSystem) override;

    bool loadCargo(const Resources& resources);
    Resources unloadCargo(const Resources& resources);
    bool isFull() const { return _cargo.total() >= _cargoCapacity; }
    bool isEmpty() const { return _cargo.total() <= 0; }    

    ShipType getShipType() const override { return ShipType::TRANSPORT; }
    char getSymbol() const override { return 'T'; }
    int getXpReward() const override { return GameConstants::TRANSPORT_XP_REWARD; }
    Resources getCargo() const { return _cargo; }
    int getCargoCapacity() const { return _cargoCapacity; }
};