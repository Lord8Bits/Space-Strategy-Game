#include "InputParser.hpp"

#include <sstream>

std::optional<Action> InputParser::parseAction(const std::string& input) const {
    std::istringstream stream(input);
    std::string action_name;
    int entity_id = -1;

    if (!(stream >> action_name >> entity_id)) {
        return std::nullopt;
    }

    const auto action_type = parseActionType(action_name);
    if (!action_type.has_value()) {
        return std::nullopt;
    }

    switch (*action_type) {
        case Action::Type::MOVE: {
            Vec2 target{};
            if (!(stream >> target.x >> target.y)) {
                return std::nullopt;
            }
            return Action(*action_type, entity_id, target);
        }
        case Action::Type::ATTACK:
        case Action::Type::LOAD_CARGO:
        case Action::Type::UNLOAD_CARGO: {
            int target_entity_id = -1;
            if (!(stream >> target_entity_id)) {
                return std::nullopt;
            }
            return Action(*action_type, entity_id, Vec2{}, target_entity_id);
        }
        case Action::Type::MINE:
        case Action::Type::RESEARCH:
        case Action::Type::BUILD: {
            int value = 0;
            stream >> value;
            return Action(*action_type, entity_id, Vec2{}, -1, value);
        }
    }

    return std::nullopt;
}

std::string InputParser::getSupportedCommands() {
    return "move <entity_id> <x> <y>, attack <entity_id> <target_id>, "
           "mine <entity_id> [value], load <entity_id> <target_id>, "
           "unload <entity_id> <target_id>, research <entity_id> [value], "
           "build <entity_id> [value]";
}

std::optional<Action::Type> InputParser::parseActionType(const std::string& action_name) {
    if (action_name == "move") {
        return Action::Type::MOVE;
    }
    if (action_name == "attack") {
        return Action::Type::ATTACK;
    }
    if (action_name == "mine") {
        return Action::Type::MINE;
    }
    if (action_name == "load") {
        return Action::Type::LOAD_CARGO;
    }
    if (action_name == "unload") {
        return Action::Type::UNLOAD_CARGO;
    }
    if (action_name == "research") {
        return Action::Type::RESEARCH;
    }
    if (action_name == "build") {
        return Action::Type::BUILD;
    }

    return std::nullopt;
}
