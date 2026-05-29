#include <iostream>
#include "Resource.hpp"

using namespace std;

int main()
{
    Resource resources(100, 50, 25);

    cout << "===== INITIAL RESOURCES =====" << endl;

    cout << "Gold: "
         << resources.getGold()
         << endl;

    cout << "Silver: "
         << resources.getSilver()
         << endl;

    cout << "Diamond: "
         << resources.getdiamond()
         << endl;

    // ADD RESOURCES

    resources.addGold(20);
    resources.addSilver(10);
    resources.adddiamond(5);

    cout << "\n===== AFTER ADDING =====" << endl;

    cout << "Gold: "
         << resources.getGold()
         << endl;

    cout << "Silver: "
         << resources.getSilver()
         << endl;

    cout << "Diamond: "
         << resources.getdiamond()
         << endl;

    // CONSUME RESOURCES

    resources.consumeGold(30);
    resources.consumeSilver(15);
    resources.consumediamond(10);

    cout << "\n===== AFTER CONSUMING =====" << endl;

    cout << "Gold: "
         << resources.getGold()
         << endl;

    cout << "Silver: "
         << resources.getSilver()
         << endl;

    cout << "Diamond: "
         << resources.getdiamond()
         << endl;

    resources.update();

    return 0;
}
