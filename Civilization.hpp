#ifndef CIVILIZATION_H
#define CIVILIZATION_H

#include <string>
#include <vector>

#include "Resource.hpp"
#include "Planet.hpp"
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
    int getPlanetCount();
    // returns the number of planets owned by the civilization
    vector<Planet> getPlanets();
    // returns civilization planets

        // SETTERS
    void setName(string n);
    // changes civilization name

        // PLANET MANAGEMENT
    void addPlanet(Planet p);
    // adds a planet to the civilization
    void removePlanet(int index);
    // removes a planet using its index

       // RESOURCE MANAGEMENT
    void addGold(int value);
    // adds gold to civilization resources
    void addSilver(int value);
    // adds silver to civilization resources
    void addSodium(int value);
    // adds sodium to civilization resources

      // UPDATE FUNCTION
    void update() override;
    // updates civilization information
};

#endif
