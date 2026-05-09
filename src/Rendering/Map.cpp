//
// Created by lorend on 5/3/26.
//

#include "../../include/Map.h"

Map::Map() = default;

Map::~Map() = default;

void Map::addChunk(Chunk&& chunk)
{
    _chunks.push_back(std::move(chunk));
}

void Map::changeSelectedChunk(const int new_selected_chunk)
{
    _selected_chunk = (new_selected_chunk < _chunks.size()) ? new_selected_chunk : 0;
}

