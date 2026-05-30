#include "../../include/CombatSystem.hpp"
#include "../../include/Ship.hpp"
#include <string>

void CombatSystem::addLog(CombatResult& result, const std::string& message) const {
    result.logs.push_back({_currentTurn, message});
}

int CombatSystem::calculateDamage(const Ship& attacker, const Entity& defender) const {
    return attacker.calculateDamageAgainst(defender);
}

CombatResult CombatSystem::resolveCombat(Ship& attacker, Entity& defender) {
    CombatResult result;

    if (!attacker.isAlive()) {
        addLog(result, "Cannot attack. " + attacker.getName() + " is destroyed!");
        return result;
    }

    if (!defender.canBeAttacked()) {
        addLog(result, defender.getName() + " cannot be attacked!");
        return result;
    }

    if (!defender.isAlive()) {
        addLog(result, defender.getName() + " is already destroyed!");
        return result;
    }

    if (defender.getCivOwner() == attacker.getCivOwner()) {
        addLog(result, "Cannot attack a target from the same civilization!");
        return result;
    }

    attacker.setState(ShipState::ATTACKING);
    addLog(result, attacker.getName() + " attacks " + defender.getName());

    const int damage = calculateDamage(attacker, defender);

    // Virtual dispatch: Fighter overrides tryDodge(), all others return false.
    if (defender.tryDodge(attacker)) {
        result.dodged = true;
        result.attackExecuted = true;
        addLog(result, defender.getName() + " dodged the attack!");
    } else {
        // Virtual dispatch: Cruiser overrides absorbDamage(), all others pass damage through.
        const int remaining = defender.absorbDamage(damage);
        result.absorbedDamage = damage - remaining;
        result.damage = remaining;
        result.attackExecuted = true;

        if (result.absorbedDamage > 0)
            addLog(result, defender.getName() + " shield absorbed " + std::to_string(result.absorbedDamage) + " damage!");

        if (result.damage > 0) {
            defender.takeDamage(result.damage);
            result.isDestroyed = !defender.isAlive();
            addLog(result, defender.getName() + " took " + std::to_string(result.damage) + " damage!");

            if (result.isDestroyed) {
                attacker.gainXp(defender.getXpReward());
                addLog(result, defender.getName() + " has been destroyed!");
                addLog(result, attacker.getName() + " gained " + std::to_string(defender.getXpReward()) + " XP!");
            }
        } else {
            addLog(result, defender.getName() + " took no damage!");
        }
    }

    // Update diplomatic relations immediately.
    Civilization* defenderOwner = defender.getCivOwner();
    Civilization* attackerOwner = attacker.getCivOwner();
    if (attackerOwner && defenderOwner) {
        attackerOwner->setRelationWith(*defenderOwner, Relation::ENEMY);
        defenderOwner->setRelationWith(*attackerOwner, Relation::ENEMY);
        addLog(result, attackerOwner->getName() + " and " + defenderOwner->getName() + " are now enemies!");
    }

    // Return attacker to IDLE immediately so the current turn reflects the post-combat state.
    if (attacker.isAlive())
        attacker.setState(ShipState::IDLE);

    return result;
}