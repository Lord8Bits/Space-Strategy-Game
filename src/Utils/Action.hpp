#pragma once
#include "Position.hpp"
#include <string>
#include <vector>

struct Action {
    enum class Type {
        MOVE,
        ATTACK,
        MINE,
        LOAD_CARGO,
        UNLOAD_CARGO,
        RESEARCH,
        BUILD,
        COLONIZE,
        VIEW_SECTOR,
        CANCEL
    };

    Type action_type;
    int entity_id;
    Vec2 target_position;
    int target_entity_id;
    int value;
    int sector_id;

    Action(Type type, int id = -1, Vec2 target = {0,0}, int target_id = -1, int val = 0, int sector = -1)
        : action_type(type), entity_id(id), target_position(target),
          target_entity_id(target_id), value(val), sector_id(sector) {}
};

struct TurnActions {
    std::vector<Action> actions;
    int turn_number;

    TurnActions() : turn_number(0) {}

    bool isEmpty() const { return actions.empty(); }
    size_t getActionCount() const { return actions.size(); }
};