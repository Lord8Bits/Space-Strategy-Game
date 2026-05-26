#ifndef CIVILIZATION_H
#define CIVILIZATION_H

#include <string>
#include <map>
#include "../src/Utils/Enums.hpp"
#include "../src/Utils/Constants.hpp"
#include <unordered_map>
#include "Resource.h"
#include "Planet.h"
#include "Updatable.hpp"
#include "Technology.h"

using namespace std;

class Civilization : public Updatable {

private:

    string name;
    Resource resources;

    // key = planet name
    // value = Planet object
    map<string, Planet> planets;
    map<int, Technology> technologies;
protected:
    std::string _name;
    CivilizationType type;
    GameUI::Color color;
    std::unordered_map<const Civilization*, Relation> _relations;
    virtual Relation defaultRelationWith(const Civilization& other) const;
public:
    Civilization(CivilizationType Type) : type(Type), color(civToColor(Type)) {}
    Civilization(CivilizationType Type);
    virtual ~Civilization() = default;

    void update() override;
    void isAllyAttacked(const Civilization& ally, const Civilization* attacker);

    GameUI::Color getColor() const { return color; }
    
    std::string getName() const { return _name; }
    CivilizationType getCivType() const { return type; }
    Relation getRelationWith(const Civilization& other) const;
    void setRelationWith(const Civilization& other, Relation relation) { _relations[&other] = relation; }

    Civilization(string n, Resource r);

    // GETTERS
    string getName();
    Resource getResources();
    int getPlanetCount();

    map<string, Planet> getPlanets();

    // SETTERS
    void setName(string n);

    // PLANET MANAGEMENT
    void addPlanet(Planet p);
    void removePlanet(string planetName);
    Planet* findPlanet(string planetName);

    // RESOURCE MANAGEMENT
    void addGold(int value);
    void addSilver(int value);
    void adddiamond(int value);

     // technology management
    void addTechnology(Technology t);
    void removeTechnology(int id);
    Technology* findTechnology(int id);
    int getTechnologyCount();

    // UPDATE
    void update() override;
};

#endif
