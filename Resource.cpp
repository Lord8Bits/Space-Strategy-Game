#include "Resource.hpp"

// CONSTRUCTOR

Resource::Resource(int g, int s, int di)
{
    gold = g;
    silver = s;
    diamond = di;
}

// GETTERS

int Resource::getGold()
{
    return gold;
}

int Resource::getSilver()
{
    return silver;
}

int Resource::getdiamond()
{
    return diamond;
}

// ADD FUNCTIONS

void Resource::addGold(int value)
{
    gold += value;
}

void Resource::addSilver(int value)
{
    silver += value;
}

void Resource::adddiamond(int value)
{
    diamond += value;
}

// CONSUME FUNCTIONS

void Resource::consumeGold(int value)
{
    if (gold >= value)
    {
        gold -= value;
    }
}

void Resource::consumeSilver(int value)
{
    if (silver >= value)
    {
        silver -= value;
    }
}

void Resource::consumediamond(int value)
{
    if (diamond >= value)
    {
        diamond -= value;
    }
}

// UPDATE

void Resource::update()
{
}
