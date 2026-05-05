//
// Created by lorend on 5/3/26.
//

#ifndef SPACE_STRATEGY_GAME_SPACEENTITY_H
#define SPACE_STRATEGY_GAME_SPACEENTITY_H

#include <string>

struct Vec2 {
    int x{};
    int y{};
};

enum Color {
    White,
    Red,
    Blue,
    Green,
};

struct Cell {
    char symbol;
    Color color;
};

class SpaceEntity {
    friend class Render;
private:
    Vec2 _pos{0,0};
    Cell _cell{'.', White};
public:
    SpaceEntity(Vec2 new_pos, Cell new_cell);
    virtual ~SpaceEntity();
};


#endif //SPACE_STRATEGY_GAME_SPACEENTITY_H