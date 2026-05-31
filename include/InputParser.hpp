#pragma once

#include "../src/Utils/Action.hpp"
#include "../src/Utils/Position.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <stdexcept>

/// @brief Converts raw text input into typed Action objects.
///
/// Each supported command is registered in a static command table (_commands)
/// that maps keyword strings to a CommandDef containing a usage string and a
/// lambda that parses the token list into an Action.  This design means adding
/// a new command only requires one new entry in the table — no if/switch chain.
///
/// Coordinate parsing:
///   Move targets are viewport-relative chess-style coordinates: letter (A–T)
///   for the Y axis and number (1–80) for the X axis, e.g. "J40" or "A1".
///   The resulting Vec2 is relative to the currently viewed sector; Game.cpp
///   translates it to absolute world coordinates using the chunk offset.
///
/// Error handling:
///   All parse errors throw std::invalid_argument.
///   Callers should wrap parseCommand() in a try/catch and display the message
///   via getErrorMessage().
class InputParser {
private:
    static constexpr int MIN_ENTITY_ID = 0;
    static constexpr int MAX_ENTITY_ID = 10000;

    int _total_sectors; ///< Total number of chunks in the map (used for sector validation)

    /// @brief Associates a usage string with a parsing lambda for one command keyword.
    struct CommandDef {
        std::string usage;                                              ///< Shown by `help`
        std::function<Action(const std::vector<std::string>&)> parser; ///< Parses token list
    };

    /// @brief Static command registry — maps keyword → CommandDef.
    static const std::unordered_map<std::string, CommandDef> _commands;

    /// @brief Split input string into lowercase tokens.
    static std::vector<std::string> tokenize(const std::string& input);

    static bool isValidEntityId(int id);
    static bool isValidCoordinate(const Vec2& pos);
    bool        isValidSector(int sector_id) const; ///< Needs _total_sectors, so not static
    static int  parseId(const std::string& token);  ///< Throws if out of range

public:
    /// @param total_sectors Total chunk count (map.getChunkRows() * map.getChunkCols())
    explicit InputParser(int total_sectors);

    /// @brief Parse a single command line and return the corresponding Action.
    /// @throws std::invalid_argument on unknown command, bad IDs, or out-of-range coords.
    Action parseCommand(const std::string& command);

    /// @brief Parse a batch of command strings; throws on first error.
    std::vector<Action> parseCommands(const std::vector<std::string>& commands);

    /// @brief Returns true if the keyword matches a registered command.
    static bool isValidCommandType(const std::string& keyword);

    /// @brief Returns the usage strings for all registered commands (shown by `help`).
    static std::vector<std::string> getSupportedCommands();

    /// @brief Wraps exception.what() in a user-friendly prefix.
    static std::string getErrorMessage(const std::exception& error);

    /// @brief Secondary validation after parsing — checks IDs and coordinate bounds.
    /// @throws std::invalid_argument if the action contains out-of-range values.
    bool validateAction(const Action& action) const;
};