#pragma once

#include "../src/Utils/Action.hpp"
#include "../src/Utils/Position.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <stdexcept>

class InputParser {
private:
    static constexpr int MIN_ENTITY_ID = 0;
    static constexpr int MAX_ENTITY_ID = 10000;

    int _total_sectors; ///< Total number of chunks in the map (for sector validation)

    struct CommandDef {
        std::string usage;
        std::function<Action(const std::vector<std::string>&)> parser;
    };

    static const std::unordered_map<std::string, CommandDef> _commands;

    static std::vector<std::string> tokenize(const std::string& input);
    static bool isValidEntityId(int id);
    static bool isValidCoordinate(const Vec2& pos);
    bool isValidSector(int sector_id) const;  // needs _total_sectors so not static
    static int parseId(const std::string& token);

public:
    /// @param total_sectors Total chunk count from map (map.getChunkRows() * map.getChunkCols())
    explicit InputParser(int total_sectors);

    Action parseCommand(const std::string& command);
    std::vector<Action> parseCommands(const std::vector<std::string>& commands);

    static bool isValidCommandType(const std::string& keyword);
    static std::vector<std::string> getSupportedCommands();
    static std::string getErrorMessage(const std::exception& error);
    bool validateAction(const Action& action) const;
};