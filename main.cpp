#include <iostream>
#include "Planet.h"
#include "Resource.h"

using namespace std;

int main() {
    // Create a Resource object
    Resource planetResources(100, 50, 25);

    // Create a Planet object
    Planet earth("Earth", planetResources, false);
