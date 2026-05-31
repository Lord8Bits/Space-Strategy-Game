#pragma once
#include "Entities/Ship.hpp"
#include "Utils/Constants.hpp"

/// @brief Heavy ranged warship — durable hull, energy shield, 3-tile attack range.
///
/// Role: long-range fire support that absorbs punishment and deals safe chip damage.
///
/// Key stats (see Constants.hpp):
///   - HP        : 300  (heaviest hull — three times a Fighter)
///   - ATK       : 25   (lower per-hit damage, compensated by range safety)
///   - Range     : 3    (can attack without entering counter-attack range)
///   - Shield    : 50   (absorbs flat damage before HP is reduced)
///   - Movement  : 7
///
/// Special mechanics:
///   - absorbDamage(): shield pool soaks incoming hits; shield depletes permanently
///     per combat (does not regenerate unless levelUp is called).
///   - canCounterAttack(): true — Cruisers retaliate when attacked at melee range
///     (dist ≤ 1); attackers who strike from range 3 do not trigger this.
class Cruiser : public Ship {
    int _shieldStrength; ///< Remaining shield HP; absorbs damage before hull takes hits

public:
    Cruiser(const std::string& Name, const Vec2& Pos, Civilization* Owner);

    /// @brief Reduce incoming damage by the remaining shield pool.
    /// @param damage  Raw damage before absorption.
    /// @return        Damage that passes through to the hull (0 if fully absorbed).
    int  absorbDamage(int damage) override;

    /// @brief Cruisers counter-attack when the attacker is at melee range (dist ≤ 1).
    bool canCounterAttack() const override { return true; }

    std::string getDetailedInfo() const override;
    void        levelUp()         override;

    int      getShieldStrength() const { return _shieldStrength; }
    ShipType getShipType()       const override { return ShipType::CRUISER; }
    std::string getSymbol()          const override { return "◆"; }
    int      getXpReward()       const override { return GameConstants::CRUISER_XP_REWARD; }
};