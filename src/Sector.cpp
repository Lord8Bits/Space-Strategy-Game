//
// Created by lorend on 5/3/26.
//

#include "../include/Sector.h"

Sector::Sector(const int x_s, const int x_e, const int y_s, const int y_e) : _x_start(x_s), _y_start(y_s), _x_end(x_e),  _y_end(y_e) {};

Sector::~Sector()
{
    for (const SpaceEntity* entity : _entities)
        delete entity;
}

void Sector::addEntity(SpaceEntity* entity)
{
    _entities.push_back(entity);
}

void Sector::removeEntity(SpaceEntity* entity)
{
    std::erase(_entities, entity);
}




