#include <iostream>
#include "Planet.h"
#include "Resource.h"

using namespace std;

int main()
{
    Resource planetResources(100, 50, 25);

    Planet earth("Earth", planetResources, false);

    cout << "===== PLANET =====" << endl;

    cout << "Name: "
         << earth.getName()
         << endl;

    cout << "Colonized: ";

    if (earth.isColonized())
        cout << "Yes" << endl;
    else
        cout << "No" << endl;

    cout << "\nResources Check: ";

    if (earth.hasResources())
        cout << "Planet still has resources." << endl;
    else
        cout << "Planet has no resources." << endl;

    cout << "\n===== RESOURCES =====" << endl;

    cout << "Gold: "
         << earth.getResources().getGold()
         << endl;

    cout << "Silver: "
         << earth.getResources().getSilver()
         << endl;

    cout << "Diamond: "
         << earth.getResources().getdiamond()
         << endl;

    earth.colonize();

    cout << "\nAfter Colonization:" << endl;

    cout << "Colonized: ";

    if (earth.isColonized())
        cout << "Yes" << endl;
    else
        cout << "No" << endl;

    earth.update();

    return 0;
}
