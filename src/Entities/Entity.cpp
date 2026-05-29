#include "../../include/Entity.hpp"
#include <iostream>

int Entity:: _next_id = 1;

Entity::Entity(const std::string& Name, const Vec2& Pos, Civilization* Owner)
    : _id(_next_id++), _name(Name), _position(Pos), _owner(Owner) {}
