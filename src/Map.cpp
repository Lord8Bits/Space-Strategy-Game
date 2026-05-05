//
// Created by lorend on 5/3/26.
//

#include "../include/Map.h"

Map::Map() = default;

Map::~Map() = default;

void Map::addSector(const Sector& sector)
{
    _sectors.push_back(sector);
}
