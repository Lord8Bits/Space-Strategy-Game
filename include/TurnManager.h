//
// Created by ilyasse-ch7 on 05/05/2026.
//

#ifndef SPACE_STRATEGY_GAME_TURNMANAGER_H
#define SPACE_STRATEGY_GAME_TURNMANAGER_H

#include "../src/Utils/Action.hpp"
#include <vector>
#include <stdexcept>

class TurnManager {
private:
    static constexpr int DEFAULT_MAX_TURNS = 300;

    int _current_turn;
    int _max_turns;
    bool _turn_in_progress;
    TurnActions _pending_actions;
    std::vector<TurnActions> _turn_history;

    bool isValidTurnState() const;
    void applyMovementPhase(const TurnActions& actions);
    void applyCombatPhase(const TurnActions& actions);
    void applyProductionPhase(const TurnActions& actions);

public:
    explicit TurnManager(int max_turns = DEFAULT_MAX_TURNS);

    void startTurn();
    void submitAction(const Action& action);
    void submitActions(const std::vector<Action>& actions);
    int executeTurn();
    void endTurn();

    int getCurrentTurn() const { return _current_turn; }
    int getMaxTurns() const { return _max_turns; }
    bool isTurnInProgress() const { return _turn_in_progress; }
    int getPendingActionCount() const { return _pending_actions.getActionCount(); }
    bool isGameOver() const { return _current_turn >= _max_turns; }

    const TurnActions& getTurnHistory(int turn_number) const;
    void clearPendingActions();
    void reset();
};


#endif //SPACE_STRATEGY_GAME_TURNMANAGER_H
