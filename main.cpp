
#include <iostream>
#include "Resource.h"

using namespace std;

int main() {
    // Create a Resource object
    Resource resources(100, 50, 25);
    // Display initial values
    cout << "INITIAL RESOURCES" << endl;

    cout << "Gold : " << resources.getGold() << endl;
    cout << "Silver : " << resources.getSilver() << endl;
    cout << "Sodium : " << resources.getSodium() << endl;

    // Add resources
    resources.addGold(20);
    resources.addSilver(10);
    resources.addSodium(5);
    cout << endl;
    cout << "AFTER ADDING RESOURCES" << endl;
    cout << "Gold : " << resources.getGold() << endl;
    cout << "Silver : " << resources.getSilver() << endl;
    cout << "Sodium : " << resources.getSodium() << endl;

    // Consume resources
    resources.consumeGold(30);
    resources.consumeSilver(15);
    resources.consumeSodium(10);
    cout << endl;
    cout << "AFTER CONSUMING RESOURCES" << endl;
    cout << "Gold: " << resources.getGold() << endl;
    cout << "Silver: " << resources.getSilver() << endl;
    cout << "Sodium: " << resources.getSodium() << endl;
    resources.update();
    return 0;
}
