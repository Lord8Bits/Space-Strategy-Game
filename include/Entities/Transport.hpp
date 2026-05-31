#pragma once
#include "Entities/Ship.hpp"
#include "Utils/Constants.hpp"
#include "Systems/Resource.hpp"

/// @brief Non-combat logistics ship — mines planets and colonizes territories.
///
/// Role: economic backbone; it is the only ship that can execute `mine` and
/// `colonize` commands.  It has no attack power and should be escorted.
///
/// Key stats (see Constants.hpp):
///   - HP        : 150
///   - ATK       : 0    (cannot deal damage — attack() always fails gracefully)
///   - Movement  : 8
///   - Cargo     : 1000 units (holds mined resources before unloading)
///
/// Colonization risk:
///   If a Transport is destroyed while standing on a planet, the attacker's
///   civilization automatically colonizes that planet (handled in Game::executeAction).
class Transport : public Ship {
    int      _cargoCapacity; ///< Maximum total resource units this ship can carry
    Resource _cargo;         ///< Currently loaded resources (gold / titanium / cadmium)

public:
    Transport(const std::string& Name, const Vec2& Pos, Civilization* Owner);

    std::string  getDetailedInfo() const override;
    CombatResult attack(Entity& target, CombatSystem& combatSystem) override;
    void         levelUp()         override;
    void         interactEntity(Entity* other, CombatSystem& combatSystem) override;

    /// @brief Always true — only Transports can mine planets and colonize.
    bool canMine() const override { return true; }

    /// @brief Load resources into cargo hold (up to remaining capacity).
    /// @return true if any amount was loaded.
    bool loadCargo(const Resource& resource);

    /// @brief Remove resources from the cargo hold.
    /// @return The amount actually unloaded (capped by what is carried).
    Resource unloadCargo(const Resource& resource);

    bool isFull()  const { return _cargo.total() >= _cargoCapacity; }
    bool isEmpty() const { return _cargo.total() <= 0; }

    ShipType getShipType()     const override { return ShipType::TRANSPORT; }
    std::string getSymbol()    const override { return "■"; }
    int      getXpReward()     const override { return GameConstants::TRANSPORT_XP_REWARD; }
    Resource getCargo()        const { return _cargo; }
    int      getCargoCapacity() const { return _cargoCapacity; }
};