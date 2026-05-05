#pragma once
#include <string>

struct Position{
    int x;
    int y;

    int distanceTo(const Position& other) const{
        return std::abs(x - other.x) + std::abs(y - other.y);
    }

    bool operator==(const Position& other) const{
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position& other) const{
        return !(*this == other);
    }
};