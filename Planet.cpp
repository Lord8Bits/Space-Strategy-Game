#include "Planet.h"

// CONSTRUCTOR

Planet::Planet(string n, Resource r, bool c)
    : resources(r)
{
    name = n;
    colonized = c;
}

// GETTERS

string Planet::getName()
{
    return name;
}

Resource Planet::getResources()
{
    return resources;
}

bool Planet::isColonized()
{
    return colonized;
}

// SETTERS

void Planet::setName(string n)
{
    name = n;
}

void Planet::setColonized(bool c)
{
    colonized = c;
}

// PLANET ACTIONS

void Planet::colonize()
{
    colonized = true;
}

bool Planet::hasResources()
{
    return resources.getGold() > 0 ||
           resources.getSilver() > 0 ||
           resources.getdiamond() > 0;
}

// UPDATE FUNCTION

void Planet::update()
{
}
