#ifndef PLANET_H
#define PLANET_H

#include <string>
#include "Resource.h"
#include "Updatable.hpp"

using namespace std;

// Planet class represents a planet in the game
class Planet : public Updatable {

private:

    string name;
    Resource resources;
    bool colonized;

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

    // UPDATE
    void update() override;
};

#endif
