#include "../include/InputParser.hpp"
#include "../src/Utils/Constants.hpp"
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
    return pos.x >= 1 && pos.x <= VIEWPORT_WIDTH
        && pos.y >= 0 && pos.y < VIEWPORT_HEIGHT;
}

/// Parse a viewport-relative coordinate token like "A20" or "t5".
/// Letter (A-T) → Y index 0-19.  Number (1-80) → X 1-based.
static Vec2 parseViewportCoord(const std::string& token) {
    if (token.size() < 2)
        throw std::invalid_argument("Coordinate must be <letter><number>, e.g. A20 or T5");
    const char letter = static_cast<char>(std::toupper(static_cast<unsigned char>(token[0])));
    if (letter < 'A' || letter > 'T')
        throw std::invalid_argument(std::string("Y must be A-T, got: ") + token[0]);
    const int x = std::stoi(token.substr(1));
    if (x < 1 || x > VIEWPORT_WIDTH)
        throw std::invalid_argument("X must be 1-" + std::to_string(VIEWPORT_WIDTH)
                                    + ", got: " + std::to_string(x));
    return Vec2{x, letter - 'A'};
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

InputParser::InputParser(int total_sectors) : _total_sectors(total_sectors) {}

const std::unordered_map<std::string, InputParser::CommandDef> InputParser::_commands = {

    {"move", {
        "move {entity_id} {coord}  (coord = letter A-T + number 1-80, e.g. A20 or T5)",
        [](const std::vector<std::string>& t) {
            if (t.size() != 3) throw std::invalid_argument("move: expected entity_id and coord (e.g. A20)");
            return Action(Action::Type::MOVE, parseId(t[1]), parseViewportCoord(t[2]));
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
        "research  (upgrades Plasma Cannons — boosts Fighter attack)",
        [](const std::vector<std::string>& t) {
            if (t.size() != 1) throw std::invalid_argument("research: no arguments needed");
            return Action(Action::Type::RESEARCH);
        }
    }},

    {"build", {
        "build {fighter|cruiser|transport}  (built at your home planet)",
        [](const std::vector<std::string>& t) {
            if (t.size() != 2) throw std::invalid_argument("build: expected ship type (fighter/cruiser/transport)");
            int ship_type = 0;
            if      (t[1] == "fighter")   ship_type = 0;
            else if (t[1] == "cruiser")   ship_type = 1;
            else if (t[1] == "transport") ship_type = 2;
            else throw std::invalid_argument("build: unknown ship type '" + t[1] + "'");
            return Action(Action::Type::BUILD, -1, Vec2{0,0}, -1, ship_type);
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