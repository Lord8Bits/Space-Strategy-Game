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
