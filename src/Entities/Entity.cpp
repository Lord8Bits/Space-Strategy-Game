#include "../../include/Entity.hpp"
#include <utility>

int Entity::_next_id = 1;

Entity::Entity(std::string name, const Vec2& pos, const char symbol, Civilization* owner)
    : _id(_next_id++),
      _name(std::move(name)),
      _position(pos),
      _symbol(symbol),
      _owner(owner)
{
}

void Entity::setPosition(const Vec2& pos)
{
    _position = pos;
}

void Entity::setCivOwner(Civilization* newOwner)
{
    _owner = newOwner;
}