#pragma once

enum class EntityType{
    SHIP,
    PLANET
};
enum class ShipType{
    FIGHTER,
    CRUISER,
    TRANSPORT
};

enum class CivilizationType{
    PLAYER,
    AGGRESSIVE,
    NEUTRAL,
    EXPANSIONIST,
    PEACEFUL
};

enum class Relation{
    ENEMY,
    NEUTRAL,
    ALLY
};

enum class ShipState{
    IDLE,
    MOVING,
    ATTACKING,
    LOADING,
    UNLOADING,
    DESTROYED
};

enum class PlanetType{
    TERRAIN,
    MINERAL,
    ENERGY
};

/// @brief Determines what stat bonuses a Technology provides when upgraded.
enum class TechType {
    PRODUCTION,  ///< Boosts resource yield (mining, cargo capacity)
    DEFENSE,     ///< Boosts ship health and shield strength
    ATTACK       ///< Boosts ship attack power and vision range
};