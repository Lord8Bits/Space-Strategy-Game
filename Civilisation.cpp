#include "Civilization.hpp"
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

Civilization::Civilization(string n, Resource r)
    : resources(r)
{
    name = n;
}

string Civilization::getName() {
    return name;
}

Resource Civilization::getResources() {
    return resources;
}

int Civilization::getPlanetCount() {
    return planets.size();
}

map<string, Planet> Civilization::getPlanets() {
    return planets;
}

void Civilization::setName(string n) {
    name = n;
}

// PLANET MANAGEMENT

void Civilization::addPlanet(Planet p) {

    planets[p.getName()] = p;
}

void Civilization::removePlanet(string planetName) {

    planets.erase(planetName);
}

Planet* Civilization::findPlanet(string planetName) {

    auto it = planets.find(planetName);

    if (it != planets.end()) {

        return &(it->second);
    }

    return nullptr;
}

// RESOURCE MANAGEMENT

void Civilization::addGold(int value) {
    resources.addGold(value);
}

void Civilization::addSilver(int value) {
    resources.addSilver(value);
}

void Civilization::adddiamond(int value) {
    resources.addSodium(value);
}

void Civilization::addTechnology(Technology t)
{
    technologies[t.getId()] = t;
}
void Civilization::removeTechnology(int id)
{
    technologies.erase(id);
}
Technology* Civilization::findTechnology(int id)
{
    auto it = technologies.find(id);

    if (it != technologies.end())
    {
        return &(it->second);
    }
    return nullptr;
}
int Civilization::getTechnologyCount()
{
    return technologies.size();
}

void Civilization::update() {

}
