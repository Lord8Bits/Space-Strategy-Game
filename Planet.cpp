#include "Planet.h"
// Constructor
Planet::Planet(string n, Resource r, bool c)
    : resources(r)
{
    name = n;
    colonized = c;
}

// Getters
string Planet::getName() {
    return name;
}

Resource Planet::getResources() {
    return resources;
}

bool Planet::isColonized() {
    return colonized;
}
// Setters
void Planet::setName(string n) {
    name = n;
}
void Planet::setColonized(bool c) {
    colonized =c;
}
// Other functions
void Planet::colonize() {
    colonized = true;
}
// Update function
void Planet::update() {
}
