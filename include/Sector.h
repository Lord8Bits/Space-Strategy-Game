//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_SECTOR_H
#define SPACE_STRATEGY_GAME_SECTOR_H

#include <vector>

#include "SpaceEntity.h"

class Sector {
    friend class Render;
private:
    int _x_start, _y_start;
    int _x_end, _y_end;
    std::vector<SpaceEntity*> _entities;
public:
    Sector(int x_s, int x_e, int y_s, int y_e);
    ~Sector();
    void addEntity(SpaceEntity* entity);
    void removeEntity(SpaceEntity* entity);
};


#endif //SPACE_STRATEGY_GAME_SECTOR_H