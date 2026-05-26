#ifndef PLANET_H
#define PLANET_H

#include <string>
#include "Resource.h"
#include "Updatable.hpp"

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
    Resource getResources();
    bool isColonized();

    // SETTERS
    void setName(string n);
    void setColonized(bool c);

    // PLANET ACTIONS
    void colonize();
    bool hasResources();

    // UPDATE FUNCTION
    void update() override;
};

#endif
