//
// Created by lorend on 5/3/26.
//

#include "../../include/Chunk.h"
#include "Entity.hpp"

Chunk::Chunk(const int x_start, const int x_end, const int y_start, const int y_end)
: _x_start(x_start), _x_end(x_end), _y_start(y_start), _y_end(y_end){}

Chunk::~Chunk() = default;

void Chunk::addEntity(std::unique_ptr<Entity> entity)
{
    _entities.insert({entity->getId(), std::move(entity)});
}

void Chunk::removeEntity(const int entity_id)
{
    _entities.erase(entity_id);
}




