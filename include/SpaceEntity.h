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

class SpaceEntity {
private:
    Vec2 pos{};
    char symbol;
    std::string color;
public:
    SpaceEntity();
    virtual ~SpaceEntity();
};


#endif //SPACE_STRATEGY_GAME_SPACEENTITY_H