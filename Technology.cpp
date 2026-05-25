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

// TECHNOLOGY ACTIONS
void Technology::unlock() {
    unlocked = true;
}
void Technology::upgrade() {
    if (canUpgrade()) {
        level++;
    }
}

bool Technology::canUpgrade() {
    return level < maxLevel;
}

int Technology::calculateUpgradeCost() {
    return researchCost * (level + 1);
}
void Technology::displayInfo() {

    cout << "\n TECHNOLOGY " << endl;

    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Level: " << level << endl;
    cout << "Max Level: " << maxLevel << endl;
    cout << "Research Cost: " << researchCost << endl;
    cout << "Unlocked: ";

    if (unlocked)
        cout << "Yes" << endl;
    else
        cout << "No" << endl;
}

