#include "../../include/CombatSystem.hpp"
#include "../../include/Ship.hpp"
#include "../../include/Fighter.hpp"
#include "../../include/Cruiser.hpp"
#include <string>

void CombatSystem::addLog(CombatResult& result, const std::string& message) const{
    result.logs.push_back({_currentTurn, message});
}

int CombatSystem::calculateDamage(const Ship& attacker, const Entity& defender) const{
    return attacker.calculateDamageAgainst(defender);
}

CombatResult CombatSystem::resolveCombat(Ship& attacker, Entity& defender){
    CombatResult result;
    
    if (!attacker.isAlive()){
        addLog(result, "Cannot attack. " + attacker.getName() + " is destroyed!");
        return result;
    }

    Ship* defenderShip = dynamic_cast<Ship*>(&defender);
    if (!defenderShip){
        addLog(result, "Cannot attack Planets!");
        return result;
    }

    if (!defender.isAlive()){
        addLog(result, defender.getName() + " already destroyed!");
        return result;
    }

    if (defender.getCivOwner() == attacker.getCivOwner()){
        addLog(result, "Cannot attack a target from the same civilization!");
        return result;
    }
    attacker.setState(ShipState::ATTACKING);
    addLog(result, attacker.getName() + " attacks " + defender.getName());

    Fighter* defenderFighter = dynamic_cast<Fighter*>(&defender);
    Cruiser* defenderCruiser = dynamic_cast<Cruiser*>(&defender);
    int damage = calculateDamage(attacker, defender);

    if (defenderFighter && defenderFighter -> tryDodge(attacker)){
        result.dodged = true;
        result.attackExecuted = true;
        addLog(result, defender.getName() + " dodged the attack!");
    }else if (defenderCruiser){
        result.damage = defenderCruiser -> absorbDamage(damage);
        result.absorbedDamage = damage - result.damage;
        result.attackExecuted = true;
        if (result.absorbedDamage > 0)
            addLog(result, defender.getName() + " shield absorbed " + std::to_string(result.absorbedDamage) + " damage!");
        if (result.damage > 0){
            defender.takeDamage(result.damage);
            result.isDestroyed = !defender.isAlive();
            addLog(result, defender.getName() + " took " + std::to_string(result.damage) + " damage!");

            if (result.isDestroyed){
                defenderCruiser -> setState(ShipState::DESTROYED);
                int xpReward = defender.getXpReward();
                attacker.gainXp(xpReward);
                addLog(result, defender.getName() + " has been destroyed successfully!");
                addLog(result, attacker.getName() + " gained " + std::to_string(xpReward) + " XP!");
            }
        }else{
            addLog(result, defender.getName() + " took no damage!");
        }
    }else{
        defender.takeDamage(damage);
        result.attackExecuted = true;
        result.damage = damage;
        result.isDestroyed = !defender.isAlive();
        addLog(result, defender.getName() + " took " + std::to_string(damage) + " damage!");

        if (result.isDestroyed){
            defenderShip -> setState(ShipState::DESTROYED);
            int xpReward = defender.getXpReward();
            attacker.gainXp(xpReward);
            addLog(result, defender.getName() + " has been destroyed successfully!");
            addLog(result, attacker.getName() + " gained " + std::to_string(xpReward) + " XP!");
        }
    }

    Civilization* defenderOwner = defender.getCivOwner();
    Civilization* attackerOwner = attacker.getCivOwner();

    if (attackerOwner && defenderOwner){
        attackerOwner -> setRelationWith(*defenderOwner, Relation::ENEMY);
        defenderOwner -> setRelationWith(*attackerOwner, Relation::ENEMY);
        addLog(result, attackerOwner -> getName() + " and " + defenderOwner -> getName() + " now are enemies!");
    }

    return result;
}