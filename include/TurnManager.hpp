#pragma once

#include "../src/Utils/Action.hpp"
#include "Map.hpp"
#include "Render.hpp"
#include "CombatSystem.hpp"
#include <vector>
#include <iostream>

/// @brief Orchestrates a full game turn: input → movement → combat → production → render.
///
/// Usage pattern each turn:
///   1. startTurn()
///   2. submitAction() / submitActions()   (from InputParser output)
///   3. executeTurn()                      (runs all phases in order)
///   4. endTurn()                          (saves to history, advances counter)
///
/// Phase order is fixed:
///   VIEW      — switch rendered chunk immediately (before other phases)
///   MOVEMENT  — move ships, update chunk indices
///   COMBAT    — resolve attacks via CombatSystem, remove destroyed entities
///   PRODUCTION — mine / load / unload / research / build (stubs for now)
///
/// TurnManager holds non-owning references to Map, Render, and CombatSystem.
/// Those objects are owned by whoever constructs TurnManager (typically main).
class TurnManager {
private:
    static constexpr int DEFAULT_MAX_TURNS = 300;

    Map&          _map;
    Render&       _renderer;
    CombatSystem& _combat;

    int  _current_turn;
    int  _max_turns;
    bool _turn_in_progress;

    TurnActions              _pending_actions;
    std::vector<TurnActions> _turn_history;

    // ── Phase handlers ────────────────────────────────────────────────────────
    void applyViewPhase      (const TurnActions& actions);
    void applyMovementPhase  (const TurnActions& actions);
    void applyCombatPhase    (const TurnActions& actions);
    void applyProductionPhase(const TurnActions& actions);

public:
    /// @param map         The game world (non-owning reference)
    /// @param renderer    Render system (non-owning reference)
    /// @param combat      Combat system (non-owning reference)
    /// @param max_turns   Game ends after this many turns (default 300)
    TurnManager(Map& map, Render& renderer, CombatSystem& combat,
                int max_turns = DEFAULT_MAX_TURNS);

    /// @brief Open a new turn and clear any stale pending actions.
    /// @throws std::runtime_error if game is already over.
    /// @throws std::logic_error   if a turn is already in progress.
    void startTurn();

    /// @brief Queue one parsed action for the current turn.
    /// @throws std::logic_error if no turn is in progress.
    void submitAction(const Action& action);

    /// @brief Queue a batch of parsed actions for the current turn.
    /// @throws std::logic_error if no turn is in progress.
    void submitActions(const std::vector<Action>& actions);

    /// @brief Run all resolution phases for the active turn.
    /// @return Number of actions that were processed.
    /// @throws std::logic_error if no turn is in progress.
    int executeTurn();

    /// @brief Save the current turn to history and advance the turn counter.
    /// @throws std::logic_error if no turn is in progress.
    void endTurn();

    /// @brief Discard all queued actions without ending the turn.
    void clearPendingActions();

    /// @brief Return the action list for a previously resolved turn.
    /// @throws std::out_of_range if turn_number is out of history range.
    const TurnActions& getTurnHistory(int turn_number) const;

    /// @brief Reset to initial state (turn 0, empty history).
    void reset();

    int  getCurrentTurn()       const { return _current_turn; }
    int  getMaxTurns()          const { return _max_turns; }
    bool isTurnInProgress()     const { return _turn_in_progress; }
    bool isGameOver()           const { return _current_turn >= _max_turns; }
    int  getPendingActionCount()const { return static_cast<int>(_pending_actions.getActionCount()); }
};