//
// Created by ilyasse-ch7 on 05/05/2026.
//

#include "Input_Parser.h"
#include <sstream>
#include <algorithm>
#include <cctype>

InputParser::InputParser() {
}

std::vector<std::string> InputParser::tokenizeInput(const std::string& input) const {
    std::vector<std::string> tokens;
    std::istringstream stream(input);
    std::string token;

    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

Action::Type InputParser::parseActionType(const std::string& action_keyword) const {
    std::string keyword_lower = action_keyword;
    std::transform(keyword_lower.begin(), keyword_lower.end(), keyword_lower.begin(), ::tolower);

    if (keyword_lower == "move")           return Action::Type::MOVE;
    if (keyword_lower == "attack")         return Action::Type::ATTACK;
    if (keyword_lower == "mine")           return Action::Type::MINE;
    if (keyword_lower == "load")           return Action::Type::LOAD_CARGO;
    if (keyword_lower == "unload")         return Action::Type::UNLOAD_CARGO;
    if (keyword_lower == "research")       return Action::Type::RESEARCH;
    if (keyword_lower == "build")          return Action::Type::BUILD;

    throw std::invalid_argument("Unknown action type: " + action_keyword);
}

Vec2 InputParser::parseCoordinates(const std::string& x_str, const std::string& y_str) const {
    try {
        int x = std::stoi(x_str);
        int y = std::stoi(y_str);

        Vec2 coord{x, y};
        if (!isValidCoordinate(coord)) {
            throw std::invalid_argument("Coordinates out of valid range");
        }
        return coord;
    } catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Invalid coordinate format: " + std::string(e.what()));
    }
}

bool InputParser::isValidEntityId(int entity_id) const {
    return entity_id >= MIN_ENTITY_ID && entity_id <= MAX_ENTITY_ID;
}

bool InputParser::isValidCoordinate(const Vec2& position) const {
    return position.x >= 0 && position.y >= 0;
}

Action InputParser::parseCommand(const std::string& command) {
    if (command.empty()) {
        throw std::invalid_argument("Command cannot be empty");
    }

    auto tokens = tokenizeInput(command);
    if (tokens.empty()) {
        throw std::invalid_argument("No tokens found in command");
    }

    Action::Type action_type = parseActionType(tokens[0]);

    switch (action_type) {
        case Action::Type::MOVE: {
            if (tokens.size() != 4) {
                throw std::invalid_argument("move: requires entity_id and coordinates (x, y)");
            }
            int entity_id = std::stoi(tokens[1]);
            if (!isValidEntityId(entity_id)) {
                throw std::invalid_argument("Entity ID out of range");
            }
            Vec2 target = parseCoordinates(tokens[2], tokens[3]);
            return Action(Action::Type::MOVE, entity_id, target);
        }

        case Action::Type::ATTACK: {
            if (tokens.size() != 3) {
                throw std::invalid_argument("attack: requires entity_id and target_id");
            }
            int entity_id = std::stoi(tokens[1]);
            int target_id = std::stoi(tokens[2]);
            if (!isValidEntityId(entity_id) || !isValidEntityId(target_id)) {
                throw std::invalid_argument("Entity ID out of range");
            }
            return Action(Action::Type::ATTACK, entity_id, Vec2{0, 0}, target_id);
        }

        case Action::Type::MINE: {
            if (tokens.size() != 2) {
                throw std::invalid_argument("mine: requires entity_id only");
            }
            int entity_id = std::stoi(tokens[1]);
            if (!isValidEntityId(entity_id)) {
                throw std::invalid_argument("Entity ID out of range");
            }
            return Action(Action::Type::MINE, entity_id, Vec2{0, 0});
        }

        case Action::Type::LOAD_CARGO: {
            if (tokens.size() < 2) {
                throw std::invalid_argument("load: requires entity_id");
            }
            int entity_id = std::stoi(tokens[1]);
            int resource_value = (tokens.size() > 2) ? std::stoi(tokens[2]) : 0;
            if (!isValidEntityId(entity_id)) {
                throw std::invalid_argument("Entity ID out of range");
            }
            return Action(Action::Type::LOAD_CARGO, entity_id, Vec2{0, 0}, -1, resource_value);
        }

        case Action::Type::UNLOAD_CARGO: {
            if (tokens.size() != 2) {
                throw std::invalid_argument("unload: requires entity_id only");
            }
            int entity_id = std::stoi(tokens[1]);
            if (!isValidEntityId(entity_id)) {
                throw std::invalid_argument("Entity ID out of range");
            }
            return Action(Action::Type::UNLOAD_CARGO, entity_id, Vec2{0, 0});
        }

        default:
            throw std::invalid_argument("Action type not yet implemented");
    }
}

std::vector<Action> InputParser::parseCommands(const std::vector<std::string>& commands) {
    std::vector<Action> actions;
    for (const auto& command : commands) {
        actions.push_back(parseCommand(command));
    }
    return actions;
}

bool InputParser::validateAction(const Action& action) const {
    if (!isValidEntityId(action.entity_id)) {
        throw std::invalid_argument("Invalid entity ID");
    }

    switch (action.action_type) {
        case Action::Type::MOVE:
            if (!isValidCoordinate(action.target_position)) {
                throw std::invalid_argument("Target position is out of bounds");
            }
            break;

        case Action::Type::ATTACK:
            if (action.target_entity_id < 0) {
                throw std::invalid_argument("Attack requires valid target ID");
            }
            if (!isValidEntityId(action.target_entity_id)) {
                throw std::invalid_argument("Target entity ID out of range");
            }
            break;

        default:
            break;
    }

    return true;
}

std::string InputParser::getErrorMessage(const std::exception& error) {
    return std::string("Parse Error: ") + error.what();
}

bool InputParser::isValidCommandType(const std::string& command_keyword) const {
    std::string keyword_lower = command_keyword;
    std::transform(keyword_lower.begin(), keyword_lower.end(), keyword_lower.begin(), ::tolower);

    return keyword_lower == "move" || keyword_lower == "attack" || 
           keyword_lower == "mine" || keyword_lower == "load" ||
           keyword_lower == "unload" || keyword_lower == "research" ||
           keyword_lower == "build";
}

std::vector<std::string> InputParser::getSupportedCommands() const {
    return {
        "move {entity_id} {x} {y}",
        "attack {entity_id} {target_id}",
        "mine {entity_id}",
        "load {entity_id} [amount]",
        "unload {entity_id}",
        "research {entity_id}",
        "build {entity_id}"
    };
}
