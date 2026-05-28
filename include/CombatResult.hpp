#pragma once
#include <vector>
#include <string>

struct CombatEntryLog {
    int turnNumber;
    std::string message;
};

struct CombatResult {
    bool attackExecuted = false;
    bool isDestroyed = false;
    int damage = 0;
    int absorbedDamage = 0;
    bool dodged = false;
    
    std::vector<CombatEntryLog> logs;
};