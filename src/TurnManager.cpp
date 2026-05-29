//
// Created by ilyasse-ch7 on 05/05/2026.
//

#include "TurnManager.h"
#include <stdexcept>

TurnManager::TurnManager(int max_turns)
    : _current_turn(0), _max_turns(max_turns), _turn_in_progress(false) {
    if (max_turns <= 0) {
        throw std::invalid_argument("max_turns must be positive");
    }
}

void TurnManager::startTurn() {
    if (isGameOver()) {
        throw std::runtime_error("Game has ended - maximum turns reached");
    }
    if (_turn_in_progress) {
        throw std::logic_error("Cannot start turn - turn already in progress");
    }
    _turn_in_progress = true;
    _pending_actions = TurnActions();
    _pending_actions.turn_number = _current_turn;
}

void TurnManager::submitAction(const Action& action) {
    if (!_turn_in_progress) {
        throw std::logic_error("Cannot submit action - turn not in progress");
    }
    _pending_actions.actions.push_back(action);
}

void TurnManager::submitActions(const std::vector<Action>& actions) {
    if (!_turn_in_progress) {
        throw std::logic_error("Cannot submit actions - turn not in progress");
    }
    for (const auto& action : actions) {
        _pending_actions.actions.push_back(action);
    }
}

int TurnManager::executeTurn() {
    if (!_turn_in_progress) {
        throw std::logic_error("Cannot execute turn - turn not in progress");
    }

    // Phase order matters: movement, then combat, then production.
    applyMovementPhase(_pending_actions);
    applyCombatPhase(_pending_actions);
    applyProductionPhase(_pending_actions);

    return static_cast<int>(_pending_actions.getActionCount());
}

void TurnManager::endTurn() {
    if (!_turn_in_progress) {
        throw std::logic_error("Cannot end turn - turn not in progress");
    }

    _turn_history.push_back(_pending_actions);
    _turn_in_progress = false;
    _current_turn++;
}

void TurnManager::clearPendingActions() {
    _pending_actions.actions.clear();
    _pending_actions.turn_number = _current_turn;
}

const TurnActions& TurnManager::getTurnHistory(int turn_number) const {
    if (turn_number < 0 || turn_number >= static_cast<int>(_turn_history.size())) {
        throw std::out_of_range("Turn number not found in history");
    }
    return _turn_history[turn_number];
}

void TurnManager::reset() {
    _current_turn = 0;
    _turn_in_progress = false;
    _pending_actions = TurnActions();
    _turn_history.clear();
}

bool TurnManager::isValidTurnState() const {
    return _current_turn >= 0 && _current_turn <= _max_turns;
}

void TurnManager::applyMovementPhase(const TurnActions& actions) {
    // TODO: Apply MOVE actions once the turn manager is connected to the world state.
}

void TurnManager::applyCombatPhase(const TurnActions& actions) {
    // TODO: Resolve ATTACK actions through CombatSystem.
}

void TurnManager::applyProductionPhase(const TurnActions& actions) {
    // TODO: Resolve mining, research, and build actions.
}
