#include "../../include/Entity.hpp"
#include "../../include/Utils/Constants.hpp"
#include <utility>

int Entity::_next_id = 1;

Entity::Entity(const EntityType type, std::string name, const Vec2& pos, const Cell cell, Civilization* owner)
    : _id(_next_id++), _name(std::move(name)), _position(pos), _cell(cell), _owner(owner) {}

int Entity::getId()              const { return _id; }
Cell Entity::getCell()           const { return _cell; }
std::string_view Entity::getName() const { return _name; }
Vec2 Entity::getPosition()       const { return _position; }

void Entity::setPosition(const Vec2& pos)
{
    // Validate against full world bounds, not just one chunk
    if (pos.x < 0 || pos.y < 0 || pos.x >= WORLD_SIZE || pos.y >= WORLD_SIZE) return;
    _position = pos;
}

void Entity::setCivOwner(Civilization* newOwner) { _owner = newOwner; }