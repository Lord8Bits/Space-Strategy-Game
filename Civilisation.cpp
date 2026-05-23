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
    planets.push_back(p); //Add the element in the end 
}
void Civilization::removePlanet(Planet p) {
    for (int i = 0; i < planets.size(); i++) {
        if (planets[i].getName() == p.getName()) {
            planets.erase(planets.begin() + i); // Earse remove the element from the vector
            break;
        }
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

