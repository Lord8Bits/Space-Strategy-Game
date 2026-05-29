#include "../include/TurnManager.hpp"
#include "../include/Ship.hpp"
#include "../include/Transport.hpp"
#include <stdexcept>

// ─── Constructor ─────────────────────────────────────────────────────────────

TurnManager::TurnManager(Map& map, Render& renderer, CombatSystem& combat, int max_turns)
    : _map(map), _renderer(renderer), _combat(combat),
      _current_turn(0), _max_turns(max_turns), _turn_in_progress(false)
{
    if (max_turns <= 0)
        throw std::invalid_argument("max_turns must be positive");
}

// ─── Turn lifecycle ───────────────────────────────────────────────────────────

void TurnManager::startTurn() {
    if (isGameOver())
        throw std::runtime_error("Game has ended - maximum turns reached");
    if (_turn_in_progress)
        throw std::logic_error("Cannot start turn - turn already in progress");

    _turn_in_progress = true;
    _pending_actions = TurnActions();
    _pending_actions.turn_number = _current_turn;

    _combat.setCurrentTurn(_current_turn);
}

void TurnManager::submitAction(const Action& action) {
    if (!_turn_in_progress)
        throw std::logic_error("Cannot submit action - turn not in progress");
    _pending_actions.actions.push_back(action);
}

void TurnManager::submitActions(const std::vector<Action>& actions) {
    if (!_turn_in_progress)
        throw std::logic_error("Cannot submit actions - turn not in progress");
    for (const auto& action : actions)
        _pending_actions.actions.push_back(action);
}

int TurnManager::executeTurn() {
    if (!_turn_in_progress)
        throw std::logic_error("Cannot execute turn - turn not in progress");

    // Phase order is fixed — do not reorder
    applyViewPhase      (_pending_actions);
    applyMovementPhase  (_pending_actions);
    applyCombatPhase    (_pending_actions);
    applyProductionPhase(_pending_actions);

    return static_cast<int>(_pending_actions.getActionCount());
}

void TurnManager::endTurn() {
    if (!_turn_in_progress)
        throw std::logic_error("Cannot end turn - turn not in progress");

    // Update all entities and civilizations
    const Chunk& chunk = _map.getSelectedChunk();
    for (const int id : chunk.getEntityIDs()) {
        Entity* e = _map.getEntity(id);
        if (e) e->update();
    }

    _turn_history.push_back(_pending_actions);
    _turn_in_progress = false;
    _current_turn++;

    // Render the result of this turn
    _renderer.drawWorld(_map);
}

void TurnManager::clearPendingActions() {
    _pending_actions.actions.clear();
    _pending_actions.turn_number = _current_turn;
}

const TurnActions& TurnManager::getTurnHistory(int turn_number) const {
    if (turn_number < 0 || turn_number >= static_cast<int>(_turn_history.size()))
        throw std::out_of_range("Turn number not found in history");
    return _turn_history[turn_number];
}

void TurnManager::reset() {
    _current_turn     = 0;
    _turn_in_progress = false;
    _pending_actions  = TurnActions();
    _turn_history.clear();
}

// ─── Phase implementations ────────────────────────────────────────────────────

void TurnManager::applyViewPhase(const TurnActions& actions) {
    // VIEW_SECTOR is handled immediately — it's a camera command, not a game-state change.
    // Sector number is 1-based; convert to 0-based col/row for Map.
    for (const auto& action : actions.actions) {
        if (action.action_type != Action::Type::VIEW_SECTOR) continue;

        const int sector  = action.sector_id - 1;
        const int col     = sector % _map.getChunkCols();
        const int row     = sector / _map.getChunkCols();
        _map.changeSelectedChunk(col, row);
    }
}

void TurnManager::applyMovementPhase(const TurnActions& actions) {
    for (const auto& action : actions.actions) {
        if (action.action_type != Action::Type::MOVE) continue;

        Entity* entity = _map.getEntity(action.entity_id);
        if (!entity) continue;

        // Only ships can move
        Ship* ship = dynamic_cast<Ship*>(entity);
        if (!ship || !ship->isAlive()) continue;

        const bool moved = ship->moveTo(action.target_position);
        if (moved)
            _map.updateEntityChunk(action.entity_id);
    }
}

void TurnManager::applyCombatPhase(const TurnActions& actions) {
    // Collect IDs destroyed this phase to remove them after iteration
    std::vector<int> destroyed_ids;

    for (const auto& action : actions.actions) {
        if (action.action_type != Action::Type::ATTACK) continue;

        Entity* attacker_entity = _map.getEntity(action.entity_id);
        Entity* defender_entity = _map.getEntity(action.target_entity_id);

        if (!attacker_entity || !defender_entity) continue;

        Ship* attacker = dynamic_cast<Ship*>(attacker_entity);
        if (!attacker || !attacker->isAlive()) continue;

        const CombatResult result = _combat.resolveCombat(*attacker, *defender_entity);

        // Print combat log to terminal
        for (const auto& log : result.logs)
            std::cout << "[Turn " << log.turnNumber << "] " << log.message << "\n";

        if (result.isDestroyed)
            destroyed_ids.push_back(action.target_entity_id);
    }

    // Remove destroyed entities from the world after all attacks are resolved
    for (const int id : destroyed_ids)
        _map.removeEntity(id);
}

void TurnManager::applyProductionPhase(const TurnActions& actions) {
    for (const auto& action : actions.actions) {
        switch (action.action_type) {

            case Action::Type::LOAD_CARGO: {
                Entity* entity = _map.getEntity(action.entity_id);
                if (!entity) break;
                Transport* transport = dynamic_cast<Transport*>(entity);
                if (!transport || !transport->isAlive()) break;
                // value field carries the total amount — split evenly for now
                // Full implementation requires Planet class (not yet merged)
                break;
            }

            case Action::Type::UNLOAD_CARGO: {
                Entity* entity = _map.getEntity(action.entity_id);
                if (!entity) break;
                Transport* transport = dynamic_cast<Transport*>(entity);
                if (!transport || !transport->isAlive()) break;
                // Unloaded resources need a destination (Planet/Civilization)
                // Full implementation requires Planet class (not yet merged)
                break;
            }

            case Action::Type::MINE:
            case Action::Type::RESEARCH:
            case Action::Type::BUILD:
                // Stubs — require Planet class and research system (not yet implemented)
                break;

            default:
                break;
        }
    }
}