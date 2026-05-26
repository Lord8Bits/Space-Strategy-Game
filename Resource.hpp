#ifndef RESOURCE_H
#define RESOURCE_H

#include "Updatable.hpp"

// Resource class manages different types of resources
class Resource : public Updatable {

private:

    int gold;
    int silver;
    int diamond;

public:

    // CONSTRUCTOR
    Resource(int g, int s, int di);

    // GETTERS
    int getGold();
    int getSilver();
    int getdiamond();

    // ADD FUNCTIONS
    void addGold(int value);
    void addSilver(int value);
    void adddiamond(int value);

    // CONSUME FUNCTIONS
    void consumeGold(int value);
    void consumeSilver(int value);
    void consumediamond(int value);

    // UPDATE
    void update() override;
};

#endif
