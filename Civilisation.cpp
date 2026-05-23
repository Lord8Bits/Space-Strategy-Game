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
// Resource management
void Civilization::addGold(int value) {
    resources.addGold(value);
}
void Civilization::addSilver(int value) {
    resources.addSilver(value);
}
void Civilization::addSodium(int value) {
    resources.addSodium(value);
}
// update function
void Civilization::update() {

}

