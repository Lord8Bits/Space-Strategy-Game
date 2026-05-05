//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_RENDER_H
#define SPACE_STRATEGY_GAME_RENDER_H
#include <array>
#include "Sector.h"
#include "SpaceEntity.h"
#include "Map.h"

class Map;

class Render {
public:
    static constexpr int WORLD_SIZE = 2400;
    static constexpr int VIEWPORT_WIDTH = 20;
    static constexpr int VIEWPORT_HEIGHT = 20;
    static constexpr int MAX_BUFFER = 8000;

private:
    std::array<Cell, WORLD_SIZE> _world{};
    std::string _frame_buffer{};

public:
    Render();
    ~Render();
    void draw(const Map& map);
    static std::string_view colorFormat(Color color) ;
};



#endif //SPACE_STRATEGY_GAME_RENDER_H