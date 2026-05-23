//
// Created by lorend on 5/3/26.
//

#include "../../include/Map.h"

Map::Map(const int num_rows, const int num_cols) : _chunk_row(num_rows), _chunk_col(num_cols)
{
    _selected_chunk = 0;
    for (int row = 0; row < _chunk_row; row++) {
        for (int col = 0; col < _chunk_col; col++) {
            _chunks.emplace_back(
                col * VIEWPORT_WIDTH,
                (col + 1) * VIEWPORT_WIDTH ,
                row * VIEWPORT_HEIGHT,
                (row+1) * VIEWPORT_HEIGHT
                );
        }
    }
}

Map::~Map() = default;


void Map::addEntity(std::unique_ptr<Entity> entity)
{
    const int chunk_idx{findChunkIndex(entity->getPosition())};
    _chunks[chunk_idx].addEntity(std::move(entity));
}

int Map::findChunkIndex(const Vec2& world_pos) const
{
    const int chunk_col_pos{world_pos.x / VIEWPORT_WIDTH};
    const int chunk_row_pos{world_pos.y / VIEWPORT_HEIGHT};
    const bool in_bounds{chunk_col_pos >= 0 && chunk_col_pos < _chunk_col
                    && chunk_row_pos >= 0 && chunk_row_pos < _chunk_row
    };
    if (!in_bounds) {
        throw std::out_of_range("Position outside world bounds");
    }
    return chunk_row_pos * _chunk_col + chunk_col_pos;
}

const Chunk& Map::getSelectedChunk() const{return _chunks[_selected_chunk] ;}

void Map::changeSelectedChunk(const int new_selected_chunk)
{
    if (new_selected_chunk < 0 || new_selected_chunk >= static_cast<int>(_chunks.size())) {
        _selected_chunk = 0;  // Default to first chunk if invalid
    } else {
        _selected_chunk = new_selected_chunk;
    }
}

