#ifndef CIVILIZATION_H
#define CIVILIZATION_H

#include <string>
#include <vector>

#include "Resource.h"
#include "Planet.h"
#include "Updatable.hpp"

using namespace std;

// represent a civilisation in the game
class Civilization : public Updatable {

private:
    // Attributes
    string name;          // civilization name
    Resource resources;         // civilization resources
    vector<Planet> planets;    // owned planets
public :
    
    // Initializes civilization information
    Civilization(string n, Resource r);

        // GETTERS
    string getName();
    // returns civilization name
    Resource getResources();
    // returns civilization resources
    vector<Planet> getPlanets();
    // returns civilization planets
