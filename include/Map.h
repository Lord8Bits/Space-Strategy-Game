//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_MAP_H
#define SPACE_STRATEGY_GAME_MAP_H
#include <vector>

#include "Sector.h"


class Map {
private:
    std::vector<Sector> sectors;
    int selected_sector{};
public:
    Map();
    ~Map();

    void render();
};


#endif //SPACE_STRATEGY_GAME_MAP_H