#pragma once

#include <string>
#include <vector>

#include "Entities/Entity.hpp"
#include "Systems/CombatResult.hpp"

class Ship;

/// @brief Stateless combat resolver — all combat in the game goes through here.
///
/// CombatSystem applies the following sequence when resolveCombat() is called:
///   1. Guard checks — attacker alive, has MP, target attackable, in range, not friendly.
///   2. Range check  — attacker must be within its attackRange (Manhattan distance).
///   3. Primary hit  — tryDodge() → absorbDamage() → takeDamage().
///   4. Counter-attack — if the defender survived, can counter, and the attacker is
///      at melee range (dist ≤ 1), the defender strikes back immediately.
///   5. Diplomacy    — both civilizations are set to ENEMY relation after any attack.
///
/// All events are recorded in CombatResult::logs so the UI can display them.
///
/// Design note: CombatSystem uses the Entity virtual interface exclusively —
/// no dynamic_cast or type checks — fulfilling the polymorphism requirement.
class CombatSystem {
    int _currentTurn = 0; ///< Stamped onto log entries for replay / history
public:
    CombatSystem() = default;
    ~CombatSystem() = default;

    /// @brief Resolve one attack from `attacker` against `defender`.
    /// @param attacker  The ship initiating the attack (its MP is set to 0 on success).
    /// @param defender  Any entity — ships or planets (planets reject the attack).
    /// @return          A CombatResult describing damage, dodge, destruction, and logs.
    CombatResult resolveCombat(Ship& attacker, Entity& defender);

    /// @brief Update the turn stamp written to combat log entries.
    void setCurrentTurn(int turn) { _currentTurn = turn; }

private:
    /// @brief Delegate to attacker's calculateDamageAgainst() (virtual dispatch).
    int calculateDamage(const Ship& attacker, const Entity& defender) const;

    /// @brief Append a timestamped message to the result's log vector.
    void addLog(CombatResult& result, const std::string& message) const;
};