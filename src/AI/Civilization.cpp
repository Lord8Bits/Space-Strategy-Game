#include "../../include/Civilization.hpp"
#include "../../include/Map.hpp"
#include "../../include/CombatSystem.hpp"
#include "../../include/Ship.hpp"
#include "../../include/Planet.hpp"

Civilization::Civilization(const std::string& name, CivilizationType type, Resource startingResources)
    : _name(name)
    , _type(type)
    , _color(civToColor(type))
    , _resources(startingResources)
    , _weapon_tech(1, "Plasma Cannons", TechType::ATTACK, 3, Resource(80, 60, 0))
{}

Relation Civilization::getRelationWith(const Civilization& other) const {
    auto it = _relations.find(&other);
    if (it != _relations.end())
        return it->second;
    return defaultRelationWith(other);
}

Relation Civilization::defaultRelationWith(const Civilization& other) const {
    switch (other.getCivType()) {
        case CivilizationType::PLAYER:
        case CivilizationType::PEACEFUL:
            return Relation::ALLY;
        case CivilizationType::AGGRESSIVE:
        case CivilizationType::EXPANSIONIST:
            return Relation::ENEMY;
        case CivilizationType::NEUTRAL:
        default:
            return Relation::NEUTRAL;
    }
}

void Civilization::isAllyAttacked(const Civilization& ally, const Civilization* attacker) {
    if (!attacker) return;
    if (getRelationWith(ally) == Relation::ALLY)
        setRelationWith(*attacker, Relation::ENEMY);
}

int Civilization::getAttackBonus()           const { return _weapon_tech.getAttackBonus(); }
bool Civilization::canUpgradeWeaponTech()     const { return _weapon_tech.canUpgrade(); }
Resource Civilization::getWeaponTechUpgradeCost() const { return _weapon_tech.getUpgradeCost(); }
void Civilization::upgradeWeaponTech()              { _weapon_tech.upgrade(); }

// ─── AI turn ──────────────────────────────────────────────────────────────────
//
// Rule-based "greedy" AI — each ship independently picks the best action
// available RIGHT NOW, with no lookahead into future turns.
//
// Decision flow (evaluated in order, first applicable wins):
//   1. If I can attack the nearest enemy this turn → attack.
//   2. Else if I still have movement points → move one step closer to my target.
//   3. Else → do nothing (wait for next turn).
//
// "Target" definition differs per civilization type:
//   AGGRESSIVE   → always the nearest player-owned entity (pure warmonger).
//   EXPANSIONIST → nearest uncolonized planet; falls back to nearest enemy if none.
//   NEUTRAL      → only reacts when an enemy is right next to it (defender).

void Civilization::takeTurn(Map& map, CombatSystem& combat) {
    if (_type == CivilizationType::PLAYER || _type == CivilizationType::PEACEFUL) return;

    // Snapshot the ID set — it can change if a ship gets destroyed mid-loop
    const std::set<int> ids_snapshot = _entity_ids;

    for (const int id : ids_snapshot) {
        Entity* e = map.getEntity(id);
        Ship*   ship = e ? e->asShip() : nullptr;
        if (!ship || !ship->isAlive()) continue;

        switch (_type) {
            case CivilizationType::AGGRESSIVE:
                aiActAggressive(*ship, map, combat);
                break;
            case CivilizationType::EXPANSIONIST:
                aiActExpansionist(*ship, map, combat);
                break;
            case CivilizationType::NEUTRAL:
                aiActNeutral(*ship, map, combat);
                break;
            default: break;
        }

        // Keep spatial index in sync after any movement
        map.updateEntityChunk(ship->getId());
    }
}

// ── AGGRESSIVE — chase and attack the nearest enemy ──────────────────────────
//
// Step 1: locate nearest enemy.
// Step 2: move toward it (spending movement points).
// Step 3: attack it (costs remaining movement points → 0).
// Reasoning: move first so we close distance, then attack from the new position.
void Civilization::aiActAggressive(Ship& ship, Map& map, CombatSystem& combat) {
    Entity* target = map.findNearestEnemy(ship.getPosition(), *this);
    if (!target) return;

    // Move closer if we have movement points and aren't already there
    if (ship.getMovementPoints() > 0 && ship.getPosition() != target->getPosition()) {
        ship.setDestination(target->getPosition());
        ship.advanceTowardDestination();
    }

    // Attack if we still have movement points left (attack costs all of them)
    if (ship.canAttack())
        ship.attack(*target, combat);
}

// ── EXPANSIONIST — claim planets first, fight second ─────────────────────────
//
// If there is an uncolonized planet within reach, head for it and colonize.
// If a closer enemy is in the way, attack opportunistically.
// If no unclaimed planets exist, fall back to aggressive behaviour.
void Civilization::aiActExpansionist(Ship& ship, Map& map, CombatSystem& combat) {
    Entity* planet_target = map.findNearestUncolonizedPlanet(ship.getPosition());
    Entity* enemy_target  = map.findNearestEnemy(ship.getPosition(), *this);

    // Decide primary target: unclaimed planet if it exists and isn't further than the enemy
    Entity* primary = nullptr;
    if (planet_target && enemy_target) {
        const int dp = ship.getPosition().distanceTo(planet_target->getPosition());
        const int de = ship.getPosition().distanceTo(enemy_target->getPosition());
        primary = (dp <= de) ? planet_target : enemy_target;
    } else {
        primary = planet_target ? planet_target : enemy_target;
    }

    if (!primary) return;

    // Move toward primary target
    if (ship.getMovementPoints() > 0 && ship.getPosition() != primary->getPosition()) {
        ship.setDestination(primary->getPosition());
        ship.advanceTowardDestination();
    }

    // If we reached an uncolonized planet, colonize it
    Planet* p = map.findPlanetAt(ship.getPosition()) ?
                map.findPlanetAt(ship.getPosition())->asPlanet() : nullptr;
    if (p && !p->isColonized()) {
        p->colonize(this);
        addEntity(p->getId());
        return;  // colonized — no need to attack this turn
    }

    // Otherwise attack any enemy that is here or adjacent
    if (enemy_target && ship.canAttack()
        && ship.getPosition().distanceTo(enemy_target->getPosition()) <= 1)
        ship.attack(*enemy_target, combat);
}

// ── NEUTRAL — pure defender: only retaliates when enemy is adjacent ───────────
//
// The neutral civ does not seek out enemies.
// It only fights if a hostile ship has already moved next to it (dist == 0 or 1).
void Civilization::aiActNeutral(Ship& ship, Map& map, CombatSystem& combat) {
    Entity* nearby = map.findNearestEnemy(ship.getPosition(), *this);
    if (!nearby) return;
    const int dist = ship.getPosition().distanceTo(nearby->getPosition());
    if (dist <= 1 && ship.canAttack())
        ship.attack(*nearby, combat);
}

void Civilization::update() {}