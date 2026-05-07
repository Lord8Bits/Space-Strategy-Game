//
// Created by lorend on 5/3/26.
//

#include "../include/Sector.h"

Chunk::Chunk() = default;

Chunk::~Chunk() = default;

void Chunk::addEntity(const int entity_id, std::unique_ptr<Entity> entity)
{
    _entities.insert({entity_id, std::move(entity)});
}

void Chunk::removeEntity(const int entity_id)
{
    _entities.erase(entity_id);
}




