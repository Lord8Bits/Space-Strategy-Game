#pragma once
#include <cstdlib>

/// @brief 2D vector structure representing position coordinates
struct Vec2 {
    int x{};  ///< X-coordinate (horizontal position)
    int y{};  ///< Y-coordinate (vertical position)

    /// @brief Calculates the distance between 2 vectors
    /// @param other The vector to be compared to
    int distanceTo(const Vec2& other) const{
        return std::abs(x - other.x) + std::abs(y - other.y);
    }
    /// @brief Checks whether the vectors are equal
    /// @param other The vector to be compared to
    bool operator==(const Vec2& other) const{
        return x == other.x && y == other.y;
    }
    /// @brief Checks whether the vectors are unequal
    /// @param other The vector to be compared to
    bool operator!=(const Vec2& other) const{
        return !(*this == other);
    }
};