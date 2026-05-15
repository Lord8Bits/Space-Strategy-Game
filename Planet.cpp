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
