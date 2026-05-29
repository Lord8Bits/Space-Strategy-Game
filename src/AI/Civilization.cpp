#include "../../include/Civilization.hpp"

Civilization::Civilization(CivilizationType Type) 
    : type(Type), color(civToColor(Type)) {}

Relation Civilization::getRelationWith(const Civilization& other) const{
    auto it = _relations.find(&other);
    if (it != _relations.end())
        return it -> second;
    return defaultRelationWith(other);
}

Relation Civilization::defaultRelationWith(const Civilization& other) const{
    switch(other.getCivType()){
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

void Civilization::isAllyAttacked(const Civilization& ally, const Civilization* attacker){
    if (!attacker) return;

    if (getRelationWith(ally) == Relation::ALLY)
        setRelationWith(*attacker, Relation::ENEMY);
}
void Civilization::update() {
    // Placeholder until civilization AI and economy are driven by the turn loop.
}
