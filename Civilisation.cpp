#include "Civilization.hpp"

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

void Civilization::update() {

}
