#pragma once
#include <cstddef>
#include "Enums.hpp"
#include "Position.hpp"
#include <string>
#include <vector>

/// Represents one command submitted by the player during a turn.
struct Action {
    enum class Type {
        MOVE,
        ATTACK,
        MINE,
        LOAD_CARGO,
        UNLOAD_CARGO,
        RESEARCH,
        BUILD
    };

    Type action_type;
    int entity_id;
    Vec2 target_position;      ///< Used by movement actions.
    int target_entity_id;      ///< Used by target-based actions such as attack.
    int value;                 ///< Optional numeric payload for resource actions.

    Action(Type type, int id, Vec2 target, int target_id = -1, int val = 0)
        : action_type(type), entity_id(id), target_position(target),
          target_entity_id(target_id), value(val) {}
};

/// Groups all actions submitted during a single turn.
struct TurnActions {
    std::vector<Action> actions;
    int turn_number;

    TurnActions() : turn_number(0) {}

    bool isEmpty() const { return actions.empty(); }
    size_t getActionCount() const { return actions.size(); }
};
