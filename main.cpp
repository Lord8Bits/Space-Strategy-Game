#include <iostream>
#include "Resource.h"
#include "Planet.h"
#include "Civilization.h"

using namespace std;

int main() {
    // Create Resources
    Resource earthResources(100, 50, 25);
    Resource marsResources(80, 40, 10);
    Resource civilizationResources(500, 300, 100);
    // Create Planets
    Planet earth("Earth", earthResources, false);
    Planet mars("Mars", marsResources, false);
    // Create Civilization
    Civilization humans("Humans", civilizationResources);
    cout << "===== CIVILIZATION =====" << endl;
    cout << "Name: " << humans.getName() << endl;
    // Add Planets
    humans.addPlanet(earth);
    humans.addPlanet(mars);
    cout << "\nAfter adding planets:" << endl;
    cout << "Planet count: "
         << humans.getPlanetCount()
         << endl;
    
