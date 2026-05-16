#include <iostream>
#include "Planet.h"
#include "Resource.h"

using namespace std;

int main() {
    // Create a Resource object
    Resource planetResources(100, 50, 25);

    // Create a Planet object
    Planet earth("Earth", planetResources, false);

     // Display planet information
    cout << " PLANET INFORMATION " << endl;
    cout << "Name : " << earth.getName() << endl;
    cout << "Colonized : ";
    if (earth.isColonized()) {
        cout << "Yes" << endl;
    }
    else {
        cout << "No" << endl;
    }

   // Display resources
    cout << endl;
    cout << "PLANET RESOURCES" << endl;

    cout << "Gold : " << earth.getResources().getGold() << endl;

    cout << "Silver : " << earth.getResources().getSilver() << endl;

    cout << "Sodium : " << earth.getResources().getSodium() << endl;

