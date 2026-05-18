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
