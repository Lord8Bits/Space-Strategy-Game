#include "Civilization.hpp"


// CONSTRUCTORS


Civilization::Civilization(CivilizationType Type)
    : type(Type), color(civToColor(Type))
{
}

Civilization::Civilization(string n, Resource r)
    : resources(r),
      type(CivilizationType::PLAYER),
      color(civToColor(CivilizationType::PLAYER))
{
    _name = n;
}


// RELATION MANAGEMENT


Relation Civilization::getRelationWith(const Civilization& other) const
{
    auto it = _relations.find(&other);

    if (it != _relations.end())
    {
        return it->second;
    }

    return defaultRelationWith(other);
}

Relation Civilization::defaultRelationWith(const Civilization& other) const
{
    switch (other.getCivType())
    {
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

void Civilization::setRelationWith(const Civilization& other,
                                   Relation relation)
{
    _relations[&other] = relation;
}

void Civilization::isAllyAttacked(const Civilization& ally,
                                  const Civilization* attacker)
{
    if (!attacker)
    {
        return;
    }

    if (getRelationWith(ally) == Relation::ALLY)
    {
        setRelationWith(*attacker, Relation::ENEMY);
    }
}


// GETTERS


string Civilization::getName() const
{
    return _name;
}

Resource Civilization::getResources()
{
    return resources;
}

int Civilization::getPlanetCount()
{
    return planets.size();
}

int Civilization::getTechnologyCount()
{
    return technologies.size();
}

map<string, Planet> Civilization::getPlanets()
{
    return planets;
}

GameUI::Color Civilization::getColor() const
{
    return color;
}

CivilizationType Civilization::getCivType() const
{
    return type;
}


// SETTERS


void Civilization::setName(string n)
{
    _name = n;
}


// PLANET MANAGEMENT


void Civilization::addPlanet(Planet p)
{
    planets.insert({p.getName(), p});
}

void Civilization::removePlanet(string planetName)
{
    planets.erase(planetName);
}

Planet* Civilization::findPlanet(string planetName)
{
    auto it = planets.find(planetName);

    if (it != planets.end())
    {
        return &(it->second);
    }

    return nullptr;
}


// TECHNOLOGY MANAGEMENT


void Civilization::addTechnology(Technology t)
{
    technologies.insert({t.getId(), t});
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


// RESOURCE MANAGEMENT


void Civilization::addGold(int value)
{
    resources.addGold(value);
}

void Civilization::addSilver(int value)
{
    resources.addSilver(value);
}

void Civilization::addSodium(int value)
{
    resources.addSodium(value);
}


// UPDATE


void Civilization::update()
{
    // Future game logic
}
