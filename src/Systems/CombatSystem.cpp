#include "../../include/CombatSystem.hpp"
#include "../../include/Ship.hpp"
#include <string>

void CombatSystem::addLog(CombatResult& result, const std::string& msg) const {
    result.logs.push_back({_currentTurn, msg});
}

int CombatSystem::calculateDamage(const Ship& attacker, const Entity& defender) const {
    return attacker.calculateDamageAgainst(defender);
}

CombatResult CombatSystem::resolveCombat(Ship& attacker, Entity& defender) {
    CombatResult result;

    if (!attacker.isAlive()) {
        addLog(result, attacker.getName() + " is destroyed and cannot attack!");
        return result;
    }
    if (!attacker.canAttack()) {
        addLog(result, attacker.getName() + " has no movement points left this turn!");
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
        addLog(result, "Cannot attack a friendly unit!");
        return result;
    }

    const int dist = attacker.getPosition().distanceTo(defender.getPosition());
    if (dist > attacker.getAttackRange()) {
        addLog(result, attacker.getName() + " is out of range (range "
               + std::to_string(attacker.getAttackRange())
               + ", distance " + std::to_string(dist) + ")!");
        return result;
    }

    attacker.setState(ShipState::ATTACKING);
    attacker.setMovementPoints(0);   // attacking costs all remaining movement
    addLog(result, attacker.getName() + " attacks " + defender.getName() + "!");

    const int damage = calculateDamage(attacker, defender);

    // ── Primary attack ────────────────────────────────────────────────────────
    if (defender.tryDodge(attacker)) {
        result.dodged        = true;
        result.attackExecuted = true;
        addLog(result, defender.getName() + " dodged the attack!");
    } else {
        const int remaining   = defender.absorbDamage(damage);
        result.absorbedDamage = damage - remaining;
        result.damage         = remaining;
        result.attackExecuted = true;

        if (result.absorbedDamage > 0)
            addLog(result, defender.getName() + " shield absorbed "
                   + std::to_string(result.absorbedDamage) + " damage!");

        if (result.damage > 0) {
            defender.takeDamage(result.damage);
            result.isDestroyed = !defender.isAlive();
            addLog(result, defender.getName() + " took " + std::to_string(result.damage) + " damage!");
        } else {
            addLog(result, defender.getName() + " took no damage!");
        }

        if (result.isDestroyed) {
            attacker.gainXp(defender.getXpReward());
            addLog(result, defender.getName() + " destroyed!  +" + std::to_string(defender.getXpReward()) + " XP");
        }
    }

    // ── Counter-attack — only at melee range (dist <= 1) ─────────────────────
    // Ranged attackers (Cruiser range 3) deal damage safely from afar.
    if (!result.isDestroyed && !result.dodged && defender.canCounterAttack()
        && attacker.isAlive() && dist <= 1) {
        const int counterPow = defender.getAttackPower();
        if (counterPow > 0) {
            addLog(result, defender.getName() + " counter-attacks!");
            const int counterRemaining = attacker.absorbDamage(counterPow);
            const int counterAbsorbed  = counterPow - counterRemaining;
            if (counterAbsorbed > 0)
                addLog(result, attacker.getName() + " shield absorbed "
                       + std::to_string(counterAbsorbed) + " counter damage!");
            if (counterRemaining > 0) {
                attacker.takeDamage(counterRemaining);
                addLog(result, attacker.getName() + " took " + std::to_string(counterRemaining) + " counter damage!");
            }
            if (!attacker.isAlive()) {
                result.attackerDestroyed = true;
                addLog(result, attacker.getName() + " destroyed by counter-attack!");
            }
        }
    }

    // ── Diplomacy ─────────────────────────────────────────────────────────────
    Civilization* defOwner = defender.getCivOwner();
    Civilization* atkOwner = attacker.getCivOwner();
    if (atkOwner && defOwner) {
        atkOwner->setRelationWith(*defOwner, Relation::ENEMY);
        defOwner->setRelationWith(*atkOwner, Relation::ENEMY);
        addLog(result, atkOwner->getName() + " and " + defOwner->getName() + " are now enemies!");
    }

    if (attacker.isAlive())
        attacker.setState(ShipState::IDLE);

    return result;
}
