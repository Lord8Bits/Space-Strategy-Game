#ifndef CIVILIZATION_H
#define CIVILIZATION_H

#include <string>
#include <map>
#include <unordered_map>

#include "../src/Utils/Enums.hpp"
#include "../src/Utils/Constants.hpp"

#include "Resource.hpp"
#include "Planet.hpp"
#include "Technology.hpp"
#include "Updatable.hpp"

using namespace std;

class Civilization : public Updatable {

private:

    Resource resources;

    map<string, Planet> planets;

    map<int, Technology> technologies;

protected:

    string _name;

    CivilizationType type;

    GameUI::Color color;

    unordered_map<const Civilization*, Relation> _relations;

    virtual Relation defaultRelationWith(const Civilization& other) const;

public:

    Civilization(CivilizationType Type);

    Civilization(string n, Resource r);

    virtual ~Civilization() = default;

    // GETTERS

    string getName() const;

    Resource getResources();

    int getPlanetCount();

    int getTechnologyCount();

    map<string, Planet> getPlanets();

    Relation getRelationWith(const Civilization& other) const;

    GameUI::Color getColor() const;

    CivilizationType getCivType() const;

    // SETTERS

    void setName(string n);

    void setRelationWith(const Civilization& other,
                         Relation relation);

    // PLANET MANAGEMENT

    void addPlanet(Planet p);

    void removePlanet(string planetName);

    Planet* findPlanet(string planetName);

    // TECHNOLOGY MANAGEMENT

    void addTechnology(Technology t);

    void removeTechnology(int id);

    Technology* findTechnology(int id);

    // RESOURCE MANAGEMENT

    void addGold(int value);

    void addSilver(int value);

    void addSodium(int value);

    // RELATION MANAGEMENT

    void isAllyAttacked(const Civilization& ally,
                       const Civilization* attacker);

    // UPDATE

    void update() override;
};

#endif
