#include "../include/InputParser.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

// ─── Helpers ────────────────────────────────────────────────────────────────

std::vector<std::string> InputParser::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream stream(input);
    std::string token;
    while (stream >> token) {
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        tokens.push_back(token);
    }
    return tokens;
}

bool InputParser::isValidEntityId(int id) {
    return id >= MIN_ENTITY_ID && id <= MAX_ENTITY_ID;
}

bool InputParser::isValidCoordinate(const Vec2& pos) {
    return pos.x >= 0 && pos.y >= 0;
}

bool InputParser::isValidSector(int sector_id) const {
    return sector_id >= 1 && sector_id <= _total_sectors;
}

int InputParser::parseId(const std::string& token) {
    int id = std::stoi(token);
    if (!isValidEntityId(id))
        throw std::invalid_argument("Entity ID out of range: " + token);
    return id;
}

Vec2 InputParser::parseCoords(const std::string& x, const std::string& y) {
    Vec2 pos{std::stoi(x), std::stoi(y)};
    if (!isValidCoordinate(pos))
        throw std::invalid_argument("Coordinates out of valid range");
    return pos;
}


InputParser::InputParser(int total_sectors) : _total_sectors(total_sectors) {}

const std::unordered_map<std::string, InputParser::CommandDef> InputParser::_commands = {

    {"move", {
        "move {entity_id} {x} {y}",
        [](const std::vector<std::string>& t) {
            if (t.size() != 4) throw std::invalid_argument("move: expected entity_id x y");
            return Action(Action::Type::MOVE, parseId(t[1]), parseCoords(t[2], t[3]));
        }
    }},

    {"attack", {
        "attack {entity_id} {target_id}",
        [](const std::vector<std::string>& t) {
            if (t.size() != 3) throw std::invalid_argument("attack: expected entity_id target_id");
            return Action(Action::Type::ATTACK, parseId(t[1]), Vec2{0,0}, parseId(t[2]));
        }
    }},

    {"mine", {
        "mine {entity_id}",
        [](const std::vector<std::string>& t) {
            if (t.size() != 2) throw std::invalid_argument("mine: expected entity_id");
            return Action(Action::Type::MINE, parseId(t[1]), Vec2{0,0});
        }
    }},

    {"load", {
        "load {entity_id} [amount]",
        [](const std::vector<std::string>& t) {
            if (t.size() < 2) throw std::invalid_argument("load: expected entity_id");
            int val = (t.size() > 2) ? std::stoi(t[2]) : 0;
            return Action(Action::Type::LOAD_CARGO, parseId(t[1]), Vec2{0,0}, -1, val);
        }
    }},

    {"unload", {
        "unload {entity_id}",
        [](const std::vector<std::string>& t) {
            if (t.size() != 2) throw std::invalid_argument("unload: expected entity_id");
            return Action(Action::Type::UNLOAD_CARGO, parseId(t[1]), Vec2{0,0});
        }
    }},

    {"research", {
        "research {entity_id}",
        [](const std::vector<std::string>& t) {
            if (t.size() != 2) throw std::invalid_argument("research: expected entity_id");
            return Action(Action::Type::RESEARCH, parseId(t[1]), Vec2{0,0});
        }
    }},

    {"build", {
        "build {entity_id}",
        [](const std::vector<std::string>& t) {
            if (t.size() != 2) throw std::invalid_argument("build: expected entity_id");
            return Action(Action::Type::BUILD, parseId(t[1]), Vec2{0,0});
        }
    }},

    {"view", {
        "view {sector_number}  (1-based, up to total map chunks)",
        [](const std::vector<std::string>& t) {
            if (t.size() != 2) throw std::invalid_argument("view: expected sector_number");
            int sector = std::stoi(t[1]);
            return Action(Action::Type::VIEW_SECTOR, -1, Vec2{0,0}, -1, 0, sector);
        }
    }},
};


Action InputParser::parseCommand(const std::string& command) {
    if (command.empty())
        throw std::invalid_argument("Command cannot be empty");

    auto tokens = tokenize(command);
    if (tokens.empty())
        throw std::invalid_argument("No tokens found in command");

    auto it = _commands.find(tokens[0]);
    if (it == _commands.end())
        throw std::invalid_argument("Unknown command: " + tokens[0]);

    Action action = it->second.parser(tokens);

    if (action.action_type == Action::Type::VIEW_SECTOR && !isValidSector(action.sector_id))
        throw std::invalid_argument("Sector " + std::to_string(action.sector_id)
            + " is out of range (1-" + std::to_string(_total_sectors) + ")");

    return action;
}

std::vector<Action> InputParser::parseCommands(const std::vector<std::string>& commands) {
    std::vector<Action> actions;
    actions.reserve(commands.size());
    for (const auto& cmd : commands)
        actions.push_back(parseCommand(cmd));
    return actions;
}

bool InputParser::isValidCommandType(const std::string& keyword) {
    std::string lower = keyword;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return _commands.count(lower) > 0;
}

std::vector<std::string> InputParser::getSupportedCommands() {
    std::vector<std::string> result;
    result.reserve(_commands.size());
    for (const auto& [keyword, def] : _commands)
        result.push_back(def.usage);
    return result;
}

std::string InputParser::getErrorMessage(const std::exception& error) {
    return std::string("Parse Error: ") + error.what();
}

bool InputParser::validateAction(const Action& action) const {
    if (action.action_type != Action::Type::VIEW_SECTOR && !isValidEntityId(action.entity_id))
        throw std::invalid_argument("Invalid entity ID");

    if (action.action_type == Action::Type::MOVE && !isValidCoordinate(action.target_position))
        throw std::invalid_argument("Target position is out of bounds");

    if (action.action_type == Action::Type::ATTACK && !isValidEntityId(action.target_entity_id))
        throw std::invalid_argument("Target entity ID out of range");

    if (action.action_type == Action::Type::VIEW_SECTOR && !isValidSector(action.sector_id))
        throw std::invalid_argument("Sector out of range");

    return true;
}