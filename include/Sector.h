//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_SECTOR_H
#define SPACE_STRATEGY_GAME_SECTOR_H

#include <vector>

#include "SpaceEntity.h"

class Sector {
private:
    int x_start, y_start;
    int x_end, y_end;
    std::vector<SpaceEntity*> entities;
public:
    Sector(int x_s, int x_e, int y_s, int y_e);
    ~Sector();

};


#endif //SPACE_STRATEGY_GAME_SECTOR_H