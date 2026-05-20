//
// Created by lorend on 5/3/26.
//

#include "../../include/Map.h"

Map::Map(const int num_rows, const int num_cols)
{
    _selected_chunk = 0;
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {
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

void Map::addChunk(Chunk&& chunk)
{
    _chunks.push_back(std::move(chunk));
}

void Map::changeSelectedChunk(const int new_selected_chunk)
{
    _selected_chunk = (new_selected_chunk < _chunks.size()) ? new_selected_chunk : 0;
}

