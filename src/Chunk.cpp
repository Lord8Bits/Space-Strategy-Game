//
// Created by lorend on 5/3/26.
//

#include "../include/Chunk.h"

Chunk::Chunk(const int idx_start, const int idx_end) : _idx_start(idx_start), _idx_end(idx_end) {}

Chunk::~Chunk() = default;

void Chunk::addEntity(std::unique_ptr<Entity> entity)
{
    _entities.insert({entity->_id, std::move(entity)});
}

void Chunk::removeEntity(const int entity_id)
{
    _entities.erase(entity_id);
}




