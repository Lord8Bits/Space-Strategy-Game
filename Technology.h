#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H
#include <string>
#include <iostream>

using namespace std;
// Technology class represents a researchable technology
// that can be unlocked and upgraded by a civilization.
class Technology {
private:
    // ATTRIBUTES

    int id;                // unique technology identifier
    string name;          // technology name
    int level;            // current level
    int maxLevel;         // maximum level allowed
    int researchCost;     // base research cost
    bool unlocked;       // unlock status
