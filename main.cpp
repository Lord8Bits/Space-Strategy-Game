#include <iostream>
#include "Technology.h"

using namespace std;

int main() {

    // Create a technology

    Technology mining(1, "Mining Technology", 1, 5, 100);

    // Display initial information

    cout << "INITIAL TECHNOLOGY" << endl;

    mining.displayInfo();

    // Unlock technology

    cout << "\nUnlocking technology..." << endl;

    mining.unlock();

    mining.displayInfo();

    // Upgrade technology

    cout << "\nUpgrading technology..." << endl;

    mining.upgrade();

    mining.displayInfo();

    // Check if upgrade is possible

    cout << "\nCan Upgrade: ";

    if (mining.canUpgrade()) {
        cout << "Yes" << endl;
    }
    else {
        cout << "No" << endl;
    }

    // Display upgrade cost

    cout << "\nNext Upgrade Cost: "
         << mining.calculateUpgradeCost()
         << endl;

    // Display bonuses

    cout << "\n===== BONUSES =====" << endl;

    cout << "Production Bonus: "
         << mining.getProductionBonus()
         << endl;

    cout << "Defense Bonus: "
         << mining.getDefenseBonus()
         << endl;

    cout << "Attack Bonus: "
         << mining.getAttackBonus()
         << endl;

    // Upgrade until max level

    cout << "\nUpgrading to maximum level..." << endl;

    while (mining.canUpgrade()) {

        mining.upgrade();
    }

    mining.displayInfo();

    cout << "\nCan Upgrade: ";

    if (mining.canUpgrade()) {
        cout << "Yes" << endl;
    }
    else {
        cout << "No" << endl;
    }

    return 0;
}
