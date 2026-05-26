#pragma once

#include "Utils/Action.hpp"
#include <optional>
#include <string>

class InputParser {
public:
    std::optional<Action> parseAction(const std::string& input) const;
    static std::string getSupportedCommands();

private:
    static std::optional<Action::Type> parseActionType(const std::string& action_name);
};
