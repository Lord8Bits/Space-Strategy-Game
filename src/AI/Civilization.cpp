#include "AI/Civilization.hpp"
#include "Rendering/Map.hpp"
#include "Systems/CombatSystem.hpp"
#include "Entities/Ship.hpp"
#include "Entities/Planet.hpp"
#include <cstdlib>

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

// ── ROAM — wander randomly when no enemy is visible ──────────────────────────
//
// Picks a new random patrol destination each time the previous one is reached.
// This makes AI ships explore the map rather than standing still.
void Civilization::aiRoam(Ship& ship, Map& map) {
    if (!ship.hasDestination() || ship.reachedDestination()) {
        const Vec2 patrol{
            rand() % map.getWorldWidth(),
            rand() % map.getWorldHeight()
        };
        ship.setDestination(patrol);
    }
    if (ship.getMovementPoints() > 0)
        ship.advanceTowardDestination();
}

// ── AGGRESSIVE — roam until an enemy enters vision range, then chase & attack ─
void Civilization::aiActAggressive(Ship& ship, Map& map, CombatSystem& combat) {
    Entity* target = map.findNearestEnemyInRange(ship.getPosition(), *this, ship.getVisionRange());
    if (!target) { aiRoam(ship, map); return; }

    const int dist = ship.getPosition().distanceTo(target->getPosition());

    // Close the gap if not yet within attack range
    if (ship.getMovementPoints() > 0 && dist > ship.getAttackRange()) {
        ship.setDestination(target->getPosition());
        ship.advanceTowardDestination();
    }

    // Attack if in range and MP remain
    if (ship.canAttack()
        && ship.getPosition().distanceTo(target->getPosition()) <= ship.getAttackRange())
        ship.attack(*target, combat);
}

// ── EXPANSIONIST — claim planets first; fight enemies only when visible ───────
void Civilization::aiActExpansionist(Ship& ship, Map& map, CombatSystem& combat) {
    Entity* planet_target = map.findNearestUncolonizedPlanet(ship.getPosition());
    Entity* enemy_target  = map.findNearestEnemyInRange(ship.getPosition(), *this, ship.getVisionRange());

    Entity* primary = nullptr;
    if (planet_target && enemy_target) {
        const int dp = ship.getPosition().distanceTo(planet_target->getPosition());
        const int de = ship.getPosition().distanceTo(enemy_target->getPosition());
        primary = (dp <= de) ? planet_target : enemy_target;
    } else {
        primary = planet_target ? planet_target : enemy_target;
    }

    if (!primary) { aiRoam(ship, map); return; }

    if (ship.getMovementPoints() > 0
        && ship.getPosition().distanceTo(primary->getPosition()) > ship.getAttackRange()) {
        ship.setDestination(primary->getPosition());
        ship.advanceTowardDestination();
    }

    // Colonize if standing on an uncolonized planet
    Planet* p = map.findPlanetAt(ship.getPosition()) ?
                map.findPlanetAt(ship.getPosition())->asPlanet() : nullptr;
    if (p && !p->isColonized()) {
        p->colonize(this);
        addEntity(p->getId());
        return;
    }

    if (enemy_target && ship.canAttack()
        && ship.getPosition().distanceTo(enemy_target->getPosition()) <= ship.getAttackRange())
        ship.attack(*enemy_target, combat);
}

// ── NEUTRAL — stand still; only attack enemies that enter attack range ────────
void Civilization::aiActNeutral(Ship& ship, Map& map, CombatSystem& combat) {
    Entity* nearby = map.findNearestEnemyInRange(ship.getPosition(), *this, ship.getVisionRange());
    if (!nearby) return;
    const int dist = ship.getPosition().distanceTo(nearby->getPosition());
    if (dist <= ship.getAttackRange() && ship.canAttack())
        ship.attack(*nearby, combat);
}

void Civilization::update() {}