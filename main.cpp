#include <iostream>

#include "Resource.h"
#include "Planet.h"
#include "Technology.h"
#include "Civilization.hpp"

using namespace std;

int main()
{
    Resource earthResources(100, 50, 25);
    Resource marsResources(80, 40, 10);

    Resource civResources(500, 300, 100);

    Planet earth("Earth", earthResources, false);
    Planet mars("Mars", marsResources, false);

    Civilization humans("Humans", civResources);

    cout << "Civilization : "
         << humans.getName()
         << endl;

    // PLANETS

    humans.addPlanet(earth);
    humans.addPlanet(mars);

    cout << "\nPlanet count : "
         << humans.getPlanetCount()
         << endl;

    Planet* foundPlanet =
        humans.findPlanet("Earth");

    if(foundPlanet != nullptr)
    {
        cout << "Found planet : "
             << foundPlanet->getName()
             << endl;
    }

    humans.removePlanet("Mars");

    cout << "Planet count after delete : "
         << humans.getPlanetCount()
         << endl;

    // RESOURCES

    humans.addGold(100);
    humans.addSilver(50);
    humans.addSodium(20);

    cout << "\nResources :" << endl;

    cout << "Gold : "
         << humans.getResources().getGold()
         << endl;

    cout << "Silver : "
         << humans.getResources().getSilver()
         << endl;

    cout << "Sodium : "
         << humans.getResources().getSodium()
         << endl;

    // TECHNOLOGIES

    Technology mining(1,
                      "Mining",
                      1,
                      5,
                      100);

    Technology laser(2,
                     "Laser",
                     1,
                     3,
                     200);

    humans.addTechnology(mining);
    humans.addTechnology(laser);

    cout << "\nTechnology count : "
         << humans.getTechnologyCount()
         << endl;

    Technology* tech =
        humans.findTechnology(1);

    if(tech != nullptr)
    {
        tech->unlock();

        tech->upgrade();

        tech->displayInfo();
    }

    humans.removeTechnology(2);

    cout << "\nTechnology count after delete : "
         << humans.getTechnologyCount()
         << endl;

    return 0;
