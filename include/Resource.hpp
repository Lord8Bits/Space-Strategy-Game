#pragma once

#include <algorithm>

/// Lightweight cargo/resource container used by Civilization and Transport.
struct Resource {
    int energy{};
    int ore{};
    int food{};

    Resource(int energy = 0, int ore = 0, int food = 0)
        : energy(energy), ore(ore), food(food) {}

    int total() const { return energy + ore + food; }

    /// Transport uses this check to validate cargo availability.
    bool operator>(const Resource& other) const {
        return energy >= other.energy
            && ore    >= other.ore
            && food   >= other.food;
    }

    bool operator>=(const Resource& other) const { return *this > other; }
    bool operator<(const Resource& other) const { return !(*this >= other); }

    bool operator==(const Resource& other) const {
        return energy == other.energy
            && ore == other.ore
            && food == other.food;
    }

    bool operator!=(const Resource& other) const { return !(*this == other); }

    Resource operator+(const Resource& other) const {
        return Resource(energy + other.energy, ore + other.ore, food + other.food);
    }

    Resource operator-(const Resource& other) const {
        return Resource(
            std::max(0, energy - other.energy),
            std::max(0, ore - other.ore),
            std::max(0, food - other.food)
        );
    }

    Resource& operator+=(const Resource& other) {
        energy += other.energy;
        ore += other.ore;
        food += other.food;
        return *this;
    }

    Resource& operator-=(const Resource& other) {
        energy = std::max(0, energy - other.energy);
        ore = std::max(0, ore - other.ore);
        food = std::max(0, food - other.food);
        return *this;
    }
};

using Resources = Resource;
