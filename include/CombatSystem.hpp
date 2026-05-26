#pragma once

#include <string>
#include <vector>

#include "Entity.hpp"
#include "CombatResult.hpp"

class Ship;

class CombatSystem{
    int _currentTurn = 0;
public:
    CombatSystem() = default;
    ~CombatSystem() = default;

    CombatResult resolveCombat(Ship& attacker, Entity& defender);
    void setCurrentTurn(int turn) { _currentTurn = turn; }
private:
    int calculateDamage(const Ship& attacker, const Entity& defender) const;
    void addLog(CombatResult& result, const std::string& message) const;
};