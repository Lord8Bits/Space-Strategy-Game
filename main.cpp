#include <iostream>

#include "Resource.h"
#include "Planet.h"
#include "Civilization.hpp"

using namespace std;

int main() {

    // Create resources

    Resource earthResources(100, 50, 25);
    Resource marsResources(80, 40, 10);
    Resource civResources(500, 300, 100);

    // Create planets

    Planet earth("Earth", earthResources, false);
    Planet mars("Mars", marsResources, false);

    // Create civilization

    Civilization humans("Humans", civResources);

    cout << "Civilization: "
         << humans.getName()
         << endl;

    // Add planets

    humans.addPlanet(earth);
    humans.addPlanet(mars);

    cout << "\nAfter adding planets:" << endl;

    cout << "Planet count: "
         << humans.getPlanetCount()
         << endl;

    // Search for a planet

    Planet* foundPlanet =
        humans.findPlanet("Earth");

    if (foundPlanet != nullptr) {

        cout << "\nPlanet found: "
             << foundPlanet->getName()
             << endl;
    }

    // Display civilization resources

    cout << "\nResources:" << endl;

    cout << "Gold: "
         << humans.getResources().getGold()
         << endl;

    cout << "Silver: "
         << humans.getResources().getSilver()
         << endl;

    cout << "Sodium: "
         << humans.getResources().getdiamond()
         << endl;

    // Add resources

    humans.addGold(100);
    humans.addSilver(50);
    humans.adddiamond(20);

    cout << "\nAfter resource update:" << endl;

    cout << "Gold: "
         << humans.getResources().getGold()
         << endl;

    cout << "Silver: "
         << humans.getResources().getSilver()
         << endl;

    cout << "Sodium: "
         << humans.getResources().getdiamond()
         << endl;

    // Remove Mars

    humans.removePlanet("Mars");

    cout << "\nAfter removing Mars:" << endl;

    cout << "Planet count: "
         << humans.getPlanetCount()
         << endl;

    // Verify Mars removal

    Planet* marsCheck =
        humans.findPlanet("Mars");

    if (marsCheck == nullptr) {

        cout << "Mars not found." << endl;
    }

    // technology
    Technology mining(1, "Mining", 1, 5, 100);
    Technology laser(2, "Laser", 1, 3, 200);

    humans.addTechnology(mining);
    humans.addTechnology(laser);

    cout << "Technology Count: "
         << humans.getTechnologyCount()
         << endl;

    // Test update

    humans.update();

    cout << "\nProgram executed successfully."
         << endl;

    return 0;
}
