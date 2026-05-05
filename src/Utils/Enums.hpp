#pragma once

enum class EntityType{
    FIGHTER,
    CRUISER,
    TRANSPORT,
    PLANET
};

enum class CivilizationType{
    PLAYER,
    AGGRESSIVE,
    NEUTRAL,
    EXPANSIONIST,
    PEACEFUL
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
    TERRAN,
    MINERAL,
    ENERGY
};
