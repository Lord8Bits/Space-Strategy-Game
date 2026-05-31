#pragma once

#include "Utils/Action.hpp"
#include "Rendering/Map.hpp"
#include "Rendering/Render.hpp"
#include "Rendering/SubjectiveRender.hpp"
#include "Systems/CombatSystem.hpp"
#include "Systems/Player.hpp"
#include <vector>
#include <iostream>

/// @brief Orchestrates a full game turn: input → movement → combat → production → FoW → render.
///
/// Usage pattern each turn:
///   1. startTurn()
///   2. submitAction() / submitActions()   (from InputParser output)
///   3. executeTurn()                      (runs all phases in order)
///   4. endTurn()                          (updates FoW, renders, advances counter)
///
/// TurnManager holds non-owning references to all systems and the active player.
/// Call setActivePlayer() when turn switches to a different player.
class TurnManager {
private:
    static constexpr int DEFAULT_MAX_TURNS = 300;

    Map&              _map;
    SubjectiveRender& _subjective_renderer;
    CombatSystem&     _combat;
    Player*           _active_player;  ///< Non-owning pointer — never null during a turn

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
    void updateFogOfWar      ();

public:
    /// @param map                 The game world (non-owning reference)
    /// @param subjective_renderer FoW-aware renderer (non-owning reference)
    /// @param combat              Combat system (non-owning reference)
    /// @param initial_player      The first player to act (non-owning pointer)
    /// @param max_turns           Game ends after this many turns (default 300)
    TurnManager(Map& map, SubjectiveRender& subjective_renderer, CombatSystem& combat,
                Player& initial_player, int max_turns = DEFAULT_MAX_TURNS);

    /// @brief Switch which player is currently acting.
    /// Call this between turns when rotating players.
    void setActivePlayer(Player& player) { _active_player = &player; }

    /// @brief Returns the currently active player.
    Player& getActivePlayer() { return *_active_player; }

    /// @brief Open a new turn and clear any stale pending actions.
    /// @throws std::runtime_error if game is already over.
    /// @throws std::logic_error   if a turn is already in progress.
    void startTurn();

    /// @brief Queue one parsed action for the current turn.
    void submitAction(const Action& action);

    /// @brief Queue a batch of parsed actions for the current turn.
    void submitActions(const std::vector<Action>& actions);

    /// @brief Run all resolution phases for the active turn.
    /// @return Number of actions processed.
    int executeTurn();

    /// @brief Update entities, recalculate FoW for active player, render, advance turn.
    void endTurn();

    void clearPendingActions();
    const TurnActions& getTurnHistory(int turn_number) const;
    void reset();

    int  getCurrentTurn()        const { return _current_turn; }
    int  getMaxTurns()           const { return _max_turns; }
    bool isTurnInProgress()      const { return _turn_in_progress; }
    bool isGameOver()            const { return _current_turn >= _max_turns; }
    int  getPendingActionCount() const { return static_cast<int>(_pending_actions.getActionCount()); }
};