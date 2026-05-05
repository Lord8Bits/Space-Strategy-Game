//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_MAP_H
#define SPACE_STRATEGY_GAME_MAP_H
#include <vector>

#include "Sector.h"


class Map {
    friend class Render;
private:
    std::vector<Sector> _sectors;
    int _selected_sector{};
public:
    Map();
    ~Map();
    void addSector(const Sector& sector);
};


#endif //SPACE_STRATEGY_GAME_MAP_H