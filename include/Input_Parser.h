//
// Created by ilyasse-ch7 on 05/05/2026.
//

#ifndef SPACE_STRATEGY_GAME_INPUT_PARSER_H
#define SPACE_STRATEGY_GAME_INPUT_PARSER_H

#include "Utils/Action.hpp"
#include "Utils/Position.hpp"
#include <string>
#include <vector>
#include <stdexcept>

class InputParser {
private:
    static constexpr int MIN_ENTITY_ID = 0;
    static constexpr int MAX_ENTITY_ID = 10000;

    std::vector<std::string> tokenizeInput(const std::string& input) const;
    bool isValidEntityId(int entity_id) const;
    bool isValidCoordinate(const Vec2& position) const;
    Action::Type parseActionType(const std::string& action_keyword) const;
    Vec2 parseCoordinates(const std::string& x_str, const std::string& y_str) const;

public:
    InputParser();

    // Parse commands: "move 5 10 20", "attack 3 7", "mine 2"
    Action parseCommand(const std::string& command);
    std::vector<Action> parseCommands(const std::vector<std::string>& commands);

    bool validateAction(const Action& action) const;
    static std::string getErrorMessage(const std::exception& error);
    bool isValidCommandType(const std::string& command_keyword) const;
    std::vector<std::string> getSupportedCommands() const;
};


#endif //SPACE_STRATEGY_GAME_INPUT_PARSER_H
