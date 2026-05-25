#ifndef CIVILIZATION_H
#define CIVILIZATION_H

#include <string>
#include <map>

#include "Resource.hpp"
#include "Planet.hpp"
#include "Updatable.hpp"

using namespace std;

class Civilization : public Updatable {

private:

    string name;
    Resource resources;

    // key = planet name
    // value = Planet object
    map<string, Planet> planets;

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
    void addSodium(int value);

    // UPDATE
    void update() override;
};

#endif
