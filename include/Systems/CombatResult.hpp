#pragma once
#include <vector>
#include <string>

/// @brief A single timestamped line in the combat event log.
struct CombatEntryLog {
    int         turnNumber; ///< Turn on which this event occurred
    std::string message;    ///< Human-readable description shown in the UI
};

/// @brief Value object returned by CombatSystem::resolveCombat().
///
/// Callers (Game::executeAction, AI act functions) inspect these flags to:
///   - Remove destroyed entities from the map and civilization registries
///   - Display the event log to the player
///   - Trigger secondary effects (e.g. colonization on Transport kill)
struct CombatResult {
    bool attackExecuted    = false; ///< True if the attack passed all guard checks
    bool isDestroyed       = false; ///< True if the defender was killed this attack
    bool attackerDestroyed = false; ///< True if the attacker was killed by counter-attack
    int  damage            = 0;     ///< Net damage dealt to the defender's hull
    int  absorbedDamage    = 0;     ///< Damage absorbed by the defender's shield
    bool dodged            = false; ///< True if the defender successfully evaded

    std::vector<CombatEntryLog> logs; ///< Ordered sequence of events for UI display
};