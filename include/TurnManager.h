//
// Created by ilyasse-ch7 on 05/05/2026.
//

#ifndef SPACE_STRATEGY_GAME_TURNMANAGER_H
#define SPACE_STRATEGY_GAME_TURNMANAGER_H

#include "../src/Utils/Action.hpp"
#include <vector>

/// Collects actions for one turn, resolves them in phase order, and keeps turn history.
class TurnManager {
private:
    static constexpr int DEFAULT_MAX_TURNS = 300;

    /// Current turn index, starting at zero.
    int _current_turn;
    int _max_turns;
    bool _turn_in_progress;
    /// Actions queued for the active turn before resolution.
    TurnActions _pending_actions;

    /// Resolved turns are stored by turn number for replay and debugging.
    std::vector<TurnActions> _turn_history;

    bool isValidTurnState() const;
    void applyMovementPhase(const TurnActions& actions);
    void applyCombatPhase(const TurnActions& actions);
    void applyProductionPhase(const TurnActions& actions);

public:
    /// Creates a turn manager with a fixed maximum number of turns.
    explicit TurnManager(int max_turns = DEFAULT_MAX_TURNS);

    /// Opens a new turn and clears any stale pending actions.
    void startTurn();
    /// Adds one parsed action to the current turn.
    void submitAction(const Action& action);
    /// Adds a batch of parsed actions to the current turn.
    void submitActions(const std::vector<Action>& actions);
    /// Runs the resolution phases for the active turn.
    int executeTurn();
    /// Stores the current turn in history and advances to the next one.
    void endTurn();

    int getCurrentTurn() const { return _current_turn; }
    int getMaxTurns() const { return _max_turns; }
    bool isTurnInProgress() const { return _turn_in_progress; }
    int getPendingActionCount() const { return _pending_actions.getActionCount(); }
    bool isGameOver() const { return _current_turn >= _max_turns; }

    /// Returns the stored action list for a resolved turn.
    const TurnActions& getTurnHistory(int turn_number) const;
    /// Clears the actions queued for the active turn.
    void clearPendingActions();
    /// Resets the manager to its initial state.
    void reset();
};


#endif //SPACE_STRATEGY_GAME_TURNMANAGER_H
