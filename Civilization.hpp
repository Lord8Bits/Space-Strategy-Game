#ifndef CIVILIZATION_H
#define CIVILIZATION_H

#include <string>
#include <map>

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

public:

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

    void addTechnology(Technology t);
    void removeTechnology(int id);
    Technology* findTechnology(int id);
    int getTechnologyCount();

    // UPDATE
    void update() override;
};

#endif
