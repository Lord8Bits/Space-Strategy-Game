#include "Technology.h"

// CONSTRUCTOR

Technology::Technology(int i, string n, int l, int maxL, int cost) {
    id = i;
    name = n;
    level = l;
    maxLevel = maxL;
    researchCost = cost;
    unlocked = false;
}

// GETTERS
int Technology::getId() {
    return id;
}
string Technology::getName() {
    return name;
}
int Technology::getLevel() {
    return level;
}
int Technology::getMaxLevel() {
    return maxLevel;
}
int Technology::getResearchCost() {
    return researchCost;
}
bool Technology::isUnlocked() {
    return unlocked;
}

// SETTERS
void Technology::setName(string n) {
    name = n;
}
void Technology::setLevel(int l) {
    level = l;
}
