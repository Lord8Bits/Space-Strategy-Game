#include "Civilization.h"

// Constructor
Civilization::Civilization(string n, Resource r)
    : resources(r)
{
    name = n;
}

// Getters
string Civilization::getName() {
    return name;
}
Resource Civilization::getResources() {
    return resources;
}
vector<Planet> Civilization::getPlanets() {
    return planets;
}

// Setters
void Civilization::setName(string n) {
    name = n;
}

//  Planet management
void Civilization::addPlanet(Planet p) {
    planets.push_back(p);
}
void Civilization::removePlanet(int index) {
    if (index >= 0 && index < planets.size()) {
        planets.erase(planets.begin() + index);
    }
}

