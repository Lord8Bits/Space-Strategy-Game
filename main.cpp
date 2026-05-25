
#include <iostream>
#include "Resource.h"

using namespace std;

int main() {
    // Create a Resource object
    Resource resources(100, 50, 25);
    // Display initial valuess
    cout << "INITIAL RESOURCES" << endl;

    cout << "Gold : " << resources.getGold() << endl;
    cout << "Silver : " << resources.getSilver() << endl;
    cout << "Sodium : " << resources.getdiamond() << endl;

    // Add resources
    resources.addGold(20);
    resources.addSilver(10);
    resources.adddiamond(5);
    cout << endl;
    
    cout << "AFTER ADDING RESOURCES" << endl;
    cout << "Gold : " << resources.getGold() << endl;
    cout << "Silver : " << resources.getSilver() << endl;
    cout << "Sodium : " << resources.getdiamond() << endl;

    // Consume resources
    resources.consumeGold(30);
    resources.consumeSilver(15);
    resources.consumediamond(10);
    cout << endl;
    
    cout << "AFTER CONSUMING RESOURCES" << endl;
    cout << "Gold: " << resources.getGold() << endl;
    cout << "Silver: " << resources.getSilver() << endl;
    cout << "Sodium: " << resources.getdiamond() << endl;
    resources.update(); // And finally the update
    return 0;
}
