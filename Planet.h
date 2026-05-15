#ifndef PLANET_H
#define PLANET_H

#include <string>
#include "Resource.h"
#include "Updatable.h"

using namespace std;

// Planet class represents a planet in the game
class Planet : public Updatable {

private:
    string name;          // planet name
    Resource resources;   // planet resources
    bool colonized;       // colonization state

public:
    // CONSTRUCTOR
    Planet(string n, Resource r, bool c);
    // GETTERS
    string getName();
    // returns the planet name

    Resource getResources();
    // returns planet resources

    bool isColonized();
    // returns colonization state
