#pragma once
#include <vector>
#include <string>

struct CombatEntryLog {
    int turnNumber;
    std::string message;
};

struct CombatResult {
    bool attackExecuted    = false;
    bool isDestroyed       = false;   ///< Defender destroyed
    bool attackerDestroyed = false;   ///< Attacker destroyed by counter-attack
    int  damage            = 0;
    int  absorbedDamage    = 0;
    bool dodged            = false;

    std::vector<CombatEntryLog> logs;
};