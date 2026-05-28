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
