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
    cout << "= CIVILIZATION =" << endl;
    cout << "Name: " << humans.getName() << endl;
    // Add Planets
    humans.addPlanet(earth);
    humans.addPlanet(mars);
    cout << "\nAfter adding planets:" << endl;
    cout << "Planet count: "
         << humans.getPlanetCount()
         << endl;
    // Display Resources
    cout << "\n= RESOURCES =" << endl;
    cout << "Gold: "
         << humans.getResources().getGold()
         << endl;

    cout << "Silver: "
         << humans.getResources().getSilver()
         << endl;

    cout << "Sodium: "
         << humans.getResources().getSodium()
         << endl ;
    // Add Resources
    humans.addGold(100);
    humans.addSilver(50);
    humans.addSodium(20);
    cout << "\nAfter adding resources:" << endl;
    cout << "Gold: "
         << humans.getResources().getGold()
         << endl;
    cout << "Silver: "
         << humans.getResources().getSilver()
         << endl;
    cout << "Sodium: "
         << humans.getResources().getSodium()
         << endl;
    // Remove Planet
    humans.removePlanet(mars);
    cout << "\nAfter removing Mars:" << endl;
    cout << "Planet count: "
         << humans.getPlanetCount()
         << endl;
     // Planet test
    cout << "\n= PLANET =" << endl;
    cout << "Name: "
         << earth.getName()
         << endl;

    cout << "Colonized: "
         << earth.isColonized()
         << endl;
    earth.colonize();
    cout << "After colonization: "
         << earth.isColonized()
         << endl;
    // Update test
    earth.update();
    humans.update();

    cout << "/n Program executed successfully " << endl ;
    return 0 ;
}
