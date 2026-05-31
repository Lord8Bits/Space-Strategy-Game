#pragma once
#include "Ship.hpp"
#include "Constants.hpp"

/// @brief Fast melee combat ship — high damage, short range, evasion ability.
///
/// Role: close-range brawler that relies on agility to dodge incoming fire.
///
/// Key stats (see Constants.hpp):
///   - HP        : 100  (lightest hull)
///   - ATK       : 50   (highest base damage)
///   - Range     : 1    (must be adjacent to attack)
///   - Movement  : 9    (fastest ship)
///   - Agility   : 40%  (base dodge chance, reduced by attacker's power)
///
/// Special mechanics:
///   - tryDodge()             : probabilistic evasion based on _agility
///   - calculateDamageAgainst(): adds the owner civilization's weapon tech bonus
///   - canCounterAttack()     : always true — Fighters retaliate at melee range
class Fighter : public Ship {
    int _agility; ///< Base dodge percentage (clamped 5–80 after attacker modifiers)

public:
    Fighter(const std::string& Name, const Vec2& Pos, Civilization* Owner);

    /// @brief Roll a random dodge check; returns true if the attack is evaded.
    /// Dodge chance = clamp(_agility - attacker.getAttackPower()/2, 5, 80).
    bool tryDodge(const Ship& attacker) const override;

    /// @brief Fighters always counter-attack when hit at melee range.
    bool canCounterAttack() const override { return true; }

    /// @brief Returns base attack power plus the owner's Plasma Cannons tech bonus.
    int  calculateDamageAgainst(const Entity& target) const override;

    std::string getDetailedInfo() const override;
    void        levelUp()         override;

    int      getAgility()  const { return _agility; }
    ShipType getShipType() const override { return ShipType::FIGHTER; }
    std::string getSymbol()    const override { return "▲"; }
    int      getXpReward() const override { return GameConstants::FIGHTER_XP_REWARD; }
};