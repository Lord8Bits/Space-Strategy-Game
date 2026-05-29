#pragma once

#include "../src/Utils/Action.hpp"
#include <optional>
#include <string>

/// Parses raw player commands into strongly typed actions.
class InputParser {
public:
    /// Converts one command line such as "move 12 5 9" into an Action.
    std::optional<Action> parseAction(const std::string& input) const;
    /// Returns the command syntax supported by the parser.
    static std::string getSupportedCommands();

private:
    /// Maps the textual verb to the corresponding action type.
    static std::optional<Action::Type> parseActionType(const std::string& action_name);
};
