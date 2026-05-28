//
// Created by lorend on 5/3/26.
//

#include "../../include/Chunk.hpp"
#include "Entity.hpp"

#include "../../include/Chunk.hpp"

Chunk::Chunk(const int x_start, const int x_end, const int y_start, const int y_end)
    : _x_start(x_start), _x_end(x_end), _y_start(y_start), _y_end(y_end) {}

void Chunk::addEntityIndex(const int entity_id)
{
    _entity_ids.insert(entity_id);
}

void Chunk::removeEntityIndex(const int entity_id)
{
    _entity_ids.erase(entity_id);
}




