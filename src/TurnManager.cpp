#include "../include/TurnManager.hpp"
#include "../include/Ship.hpp"
#include "../include/Transport.hpp"
#include <stdexcept>

// ─── Constructor ─────────────────────────────────────────────────────────────

TurnManager::TurnManager(Map& map, SubjectiveRender& subjective_renderer,
                         CombatSystem& combat, Player& initial_player, int max_turns)
    : _map(map)
    , _subjective_renderer(subjective_renderer)
    , _combat(combat)
    , _active_player(&initial_player)
    , _current_turn(0)
    , _max_turns(max_turns)
    , _turn_in_progress(false)
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
    _pending_actions  = TurnActions();
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

    applyViewPhase      (_pending_actions);
    applyMovementPhase  (_pending_actions);
    applyCombatPhase    (_pending_actions);
    applyProductionPhase(_pending_actions);

    return static_cast<int>(_pending_actions.getActionCount());
}

void TurnManager::endTurn() {
    if (!_turn_in_progress)
        throw std::logic_error("Cannot end turn - turn not in progress");

    // Step 1: Tick update() on every entity across all chunks
    for (int row = 0; row < _map.getChunkRows(); ++row) {
        for (int col = 0; col < _map.getChunkCols(); ++col) {
            _map.changeSelectedChunk(col, row);
            for (const int id : _map.getSelectedChunk().getEntityIDs()) {
                Entity* e = _map.getEntity(id);
                if (e) e->update();
            }
        }
    }

    // Step 2: Restore selected chunk to what the player was viewing
    // (the VIEW phase already set it; the update loop above may have changed it)
    // We re-apply the last VIEW_SECTOR action if any were submitted
    applyViewPhase(_pending_actions);

    // Step 3: Recalculate fog of war for the active player
    updateFogOfWar();

    // Step 4: Render the world through the active player's FoW
    _subjective_renderer.draw(*_active_player, _map);

    // Step 5: Commit turn to history and advance
    _turn_history.push_back(_pending_actions);
    _turn_in_progress = false;
    _current_turn++;
}

void TurnManager::updateFogOfWar() {
    _active_player->refreshFogOfWar(_map);
}

// ─── Phase implementations ────────────────────────────────────────────────────

void TurnManager::applyViewPhase(const TurnActions& actions) {
    for (const auto& action : actions.actions) {
        if (action.action_type != Action::Type::VIEW_SECTOR) continue;
        const int sector = action.sector_id - 1;
        const int col    = sector % _map.getChunkCols();
        const int row    = sector / _map.getChunkCols();
        _map.changeSelectedChunk(col, row);
    }
}

void TurnManager::applyMovementPhase(const TurnActions& actions) {
    for (const auto& action : actions.actions) {
        if (action.action_type != Action::Type::MOVE) continue;

        Entity* entity = _map.getEntity(action.entity_id);
        if (!entity) continue;

        Ship* ship = entity->asShip();
        if (!ship || !ship->isAlive()) continue;

        if (ship->moveTo(action.target_position))
            _map.updateEntityChunk(action.entity_id);
    }
}

void TurnManager::applyCombatPhase(const TurnActions& actions) {
    std::vector<int> destroyed_ids;

    for (const auto& action : actions.actions) {
        if (action.action_type != Action::Type::ATTACK) continue;

        Entity* attacker_entity = _map.getEntity(action.entity_id);
        Entity* defender_entity = _map.getEntity(action.target_entity_id);
        if (!attacker_entity || !defender_entity) continue;

        Ship* attacker = attacker_entity->asShip();
        if (!attacker || !attacker->isAlive()) continue;

        const CombatResult result = _combat.resolveCombat(*attacker, *defender_entity);

        for (const auto& log : result.logs)
            std::cout << "[Turn " << log.turnNumber << "] " << log.message << "\n";

        if (result.isDestroyed) {
            destroyed_ids.push_back(action.target_entity_id);
            // Also remove from the active player's fleet tracking if it was their ship
            _active_player->removeShipId(action.target_entity_id);
        }
    }

    for (const int id : destroyed_ids)
        _map.removeEntity(id);
}

void TurnManager::applyProductionPhase(const TurnActions& actions) {
    for (const auto& action : actions.actions) {
        switch (action.action_type) {
            case Action::Type::LOAD_CARGO:
            case Action::Type::UNLOAD_CARGO:
            case Action::Type::MINE:
            case Action::Type::RESEARCH:
            case Action::Type::BUILD:
                // Stubs — require Planet class (not yet merged)
                break;
            default:
                break;
        }
    }
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