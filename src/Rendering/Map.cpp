#include "Rendering/Map.hpp"
#include "Entities/Planet.hpp"
#include <stdexcept>
#include <limits>

Map::Map(const int num_rows, const int num_cols)
    : _chunk_row(num_rows), _chunk_col(num_cols)
{
    _selected_chunk = 0;
    _chunks.reserve(num_rows * num_cols);

    for (int row = 0; row < _chunk_row; row++) {
        for (int col = 0; col < _chunk_col; col++) {
            _chunks.emplace_back(
                col * VIEWPORT_WIDTH,
                (col + 1) * VIEWPORT_WIDTH,
                row * VIEWPORT_HEIGHT,
                (row + 1) * VIEWPORT_HEIGHT
            );
        }
    }
}

Map::~Map() = default;

int Map::addEntity(std::unique_ptr<Entity> entity)
{
    const int id       = entity->getId();
    const int chunk_idx = findChunkIndex(entity->getPosition());

    // Register in chunk index
    _chunks[chunk_idx].addEntityIndex(id);

    // Transfer ownership to central registry
    _all_entities.emplace(id, std::move(entity));

    return id;
}

void Map::removeEntity(const int entity_id)
{
    auto it = _all_entities.find(entity_id);
    if (it == _all_entities.end()) return;

    // Remove from chunk index first
    const int chunk_idx = findChunkIndex(it->second->getPosition());
    _chunks[chunk_idx].removeEntityIndex(entity_id);

    // Delete from central registry (unique_ptr destroyed here)
    _all_entities.erase(it);
}

Entity* Map::getEntity(const int entity_id)
{
    auto it = _all_entities.find(entity_id);
    return (it != _all_entities.end()) ? it->second.get() : nullptr;
}

const Entity* Map::getEntity(const int entity_id) const
{
    auto it = _all_entities.find(entity_id);
    return (it != _all_entities.end()) ? it->second.get() : nullptr;
}

void Map::updateEntityChunk(const int entity_id)
{
    Entity* entity = getEntity(entity_id);
    if (!entity) return;

    // Find current chunk index from old position stored in chunk indices
    // Search all chunks for this ID to find where it currently is
    for (auto& chunk : _chunks) {
        if (chunk.getEntityIDs().count(entity_id)) {
            chunk.removeEntityIndex(entity_id);
            break;
        }
    }

    // Add to new chunk based on current position
    const int new_chunk_idx = findChunkIndex(entity->getPosition());
    _chunks[new_chunk_idx].addEntityIndex(entity_id);
}

int Map::findChunkIndex(const Vec2& world_pos) const
{
    const int chunk_col = world_pos.x / VIEWPORT_WIDTH;
    const int chunk_row = world_pos.y / VIEWPORT_HEIGHT;

    const bool in_bounds = (chunk_col >= 0 && chunk_col < _chunk_col
                         && chunk_row >= 0 && chunk_row < _chunk_row);
    if (!in_bounds) {
        throw std::out_of_range("Position outside world bounds");
    }

    return chunk_row * _chunk_col + chunk_col;
}

const Chunk& Map::getSelectedChunk() const
{
    return _chunks[_selected_chunk];
}

int Map::sectorOf(const Vec2& pos) const {
    const int col = pos.x / VIEWPORT_WIDTH;
    const int row = pos.y / VIEWPORT_HEIGHT;
    return row * _chunk_col + col + 1;
}

std::string Map::toViewportCoord(const Vec2& world_pos) const {
    const int local_x = (world_pos.x % VIEWPORT_WIDTH) + 1;       // 1-80
    const int local_y =  world_pos.y % VIEWPORT_HEIGHT;            // 0-19
    return std::string(1, static_cast<char>('A' + local_y)) + std::to_string(local_x);
}

Entity* Map::findNearestEnemy(const Vec2& from, const Civilization& myCiv) const {
    Entity* nearest = nullptr;
    int minDist = std::numeric_limits<int>::max();
    for (const auto& [id, entity] : _all_entities) {
        if (!entity->isAlive()) continue;
        if (!entity->canBeAttacked()) continue;  // ignore planets and unkillable entities
        if (!entity->getCivOwner() || entity->getCivOwner() == &myCiv) continue;
        const int dist = entity->getPosition().distanceTo(from);
        if (dist < minDist) { minDist = dist; nearest = entity.get(); }
    }
    return nearest;
}

Entity* Map::findNearestEnemyInRange(const Vec2& from, const Civilization& myCiv, int range) const {
    Entity* nearest = nullptr;
    int minDist = range + 1;
    for (const auto& [id, entity] : _all_entities) {
        if (!entity->isAlive()) continue;
        if (!entity->canBeAttacked()) continue;
        if (!entity->getCivOwner() || entity->getCivOwner() == &myCiv) continue;
        const int dist = entity->getPosition().distanceTo(from);
        if (dist <= range && dist < minDist) { minDist = dist; nearest = entity.get(); }
    }
    return nearest;
}

Entity* Map::findNearestUncolonizedPlanet(const Vec2& from) const {
    Entity* nearest = nullptr;
    int minDist = std::numeric_limits<int>::max();
    for (const auto& [id, entity] : _all_entities) {
        const Planet* p = entity->asPlanet();
        if (!p || p->isColonized()) continue;
        const int dist = entity->getPosition().distanceTo(from);
        if (dist < minDist) { minDist = dist; nearest = entity.get(); }
    }
    return nearest;
}

Entity* Map::findPlanetAt(const Vec2& pos) const {
    const int chunk_idx = findChunkIndex(pos);
    for (const int id : _chunks[chunk_idx].getEntityIDs()) {
        auto it = _all_entities.find(id);
        if (it != _all_entities.end()
            && it->second->getPosition() == pos
            && it->second->asPlanet() != nullptr)
            return it->second.get();
    }
    return nullptr;
}

void Map::changeSelectedChunk(const int chunk_x, const int chunk_y)
{
    const bool in_bounds = (chunk_x >= 0 && chunk_x < _chunk_col
                         && chunk_y >= 0 && chunk_y < _chunk_row);
    if (!in_bounds) {
        throw std::out_of_range("Chunk grid coordinates out of bounds");
    }
    _selected_chunk = chunk_y * _chunk_col + chunk_x;
}