//
// Created by lorend on 5/3/26.
//

#include "../include/Entity.h"

int Entity::_next_id = 0;

Entity::Entity(const Vec2 new_pos, const Cell new_cell) : _pos(new_pos), _cell(new_cell)
{
    _id = _next_id++;
};

Entity::~Entity() {

};

