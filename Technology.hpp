#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H

#include <string>
#include <iostream>

using namespace std;

// Technology class represents a researchable technology
class Technology {

private:

    int id;
    string name;
    int level;
    int maxLevel;
    int researchCost;
    bool unlocked;

public:

    // CONSTRUCTOR
    Technology(int i, string n, int l, int maxL, int cost);

    // GETTERS
    int getId();
    string getName();
    int getLevel();
    int getMaxLevel();
    int getResearchCost();
    bool isUnlocked();

    // SETTERS
    void setName(string n);
    void setLevel(int l);

    // TECHNOLOGY ACTIONS
    void unlock();
    void upgrade();
    bool canUpgrade();
    int calculateUpgradeCost();
    void displayInfo();

    // BONUS SYSTEM
    int getProductionBonus();
    int getDefenseBonus();
    int getAttackBonus();
};

#endif
