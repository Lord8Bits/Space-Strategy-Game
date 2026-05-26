#include <iostream>
#include "Technology.hpp"

using namespace std;

int main()
{
    Technology mining(1, "Mining Technology", 1, 5, 100);

    mining.displayInfo();

    mining.unlock();

    mining.upgrade();

    cout << "\nAfter upgrade:\n";

    mining.displayInfo();

    cout << "\nProduction Bonus: "
         << mining.getProductionBonus()
         << endl;

    return 0;
}
