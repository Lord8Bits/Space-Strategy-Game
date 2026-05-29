#include "InputParser.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace {
bool isNonNegative(int value) {
    return value >= 0;
}
}

std::optional<Action> InputParser::parseAction(const std::string& input) const {
    std::istringstream stream(input);
    std::string action_name;
    int entity_id = -1;

    if (!(stream >> action_name >> entity_id) || !isNonNegative(entity_id)) {
        return std::nullopt;
    }

    const auto action_type = parseActionType(action_name);
    if (!action_type.has_value()) {
        return std::nullopt;
    }

    auto hasNoTrailingTokens = [&stream]() {
        stream >> std::ws;
        return stream.eof();
    };

    // Each action type expects a different payload after the entity id.
    switch (*action_type) {
        case Action::Type::MOVE: {
            Vec2 target{};
            if (!(stream >> target.x >> target.y) || !isNonNegative(target.x) ||
                !isNonNegative(target.y) || !hasNoTrailingTokens()) {
                return std::nullopt;
            }
            return Action(*action_type, entity_id, target);
        }
        case Action::Type::ATTACK:
        case Action::Type::LOAD_CARGO:
        case Action::Type::UNLOAD_CARGO: {
            int target_entity_id = -1;
            if (!(stream >> target_entity_id) || !isNonNegative(target_entity_id) ||
                !hasNoTrailingTokens()) {
                return std::nullopt;
            }
            return Action(*action_type, entity_id, Vec2{}, target_entity_id);
        }
        case Action::Type::MINE:
        case Action::Type::RESEARCH:
        case Action::Type::BUILD: {
            int value = 0;
            stream >> std::ws;
            if (!stream.eof()) {
                if (!(stream >> value) || !isNonNegative(value) || !hasNoTrailingTokens()) {
                    return std::nullopt;
                }
            }
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
    std::string normalized = action_name;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });

    if (normalized == "move") {
        return Action::Type::MOVE;
    }
    if (normalized == "attack") {
        return Action::Type::ATTACK;
    }
    if (normalized == "mine") {
        return Action::Type::MINE;
    }
    if (normalized == "load") {
        return Action::Type::LOAD_CARGO;
    }
    if (normalized == "unload") {
        return Action::Type::UNLOAD_CARGO;
    }
    if (normalized == "research") {
        return Action::Type::RESEARCH;
    }
    if (normalized == "build") {
        return Action::Type::BUILD;
    }

    return std::nullopt;
}
