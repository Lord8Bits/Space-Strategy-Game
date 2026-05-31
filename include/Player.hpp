#pragma once

#include "Perception.hpp"
#include "Map.hpp"
#include "Enums.hpp"
#include <string>
#include <set>

class Civilization;

/// @brief Represents a human or AI player in the game.
///
/// Ownership model (consistent with Map/Civilization/Chunk):
///   Player does NOT own ships — Map does.
///   Player stores a std::set<int> of entity IDs for its fleet.
///   Player owns its Perception object (fog of war state).
///
/// To update FoW each turn, call updateFogOfWar(map) which
/// reads ship positions directly from Map via entity IDs.
class Player {
public:
    Player(int numericId, std::string playerId, std::string displayName, bool isAI = false)
        : _numericId(numericId)
        , _playerId(std::move(playerId))
        , _displayName(std::move(displayName))
        , _isAI(isAI)
    {}

    Player(const Player&)            = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&)                 = default;
    Player& operator=(Player&&)      = default;

    // ── Identity ──────────────────────────────────────────────────────────────

    int                getNumericId()   const { return _numericId;    }
    const std::string& getId()          const { return _playerId;     }
    const std::string& getDisplayName() const { return _displayName;  }
    bool               isAI()          const { return _isAI;         }
    bool               isHuman()       const { return !_isAI;        }

    // ── Fleet tracking (IDs only — no ownership) ─────────────────────────────

    /// @brief Register an entity ID as belonging to this player's fleet.
    void addShipId(int entity_id)    { _ship_ids.insert(entity_id); }

    /// @brief Remove an entity ID from this player's fleet (e.g. on destruction).
    void removeShipId(int entity_id) { _ship_ids.erase(entity_id);  }

    /// @brief Read-only access to the set of ship IDs.
    const std::set<int>& getShipIds() const { return _ship_ids; }

    /// @brief Returns true if the player has no ships registered.
    bool isDefeated() const { return _ship_ids.empty(); }
    // ── Status ────────────────────────────────────────────────────────────────

    /// @brief Print a formatted status block for this player.
    /// @param map  Used to look up ship details by ID.
    void printStatus(const Map& map) const;
    // ── Fog of War ────────────────────────────────────────────────────────────

    /// @brief Read-only access to the player's perception state.
    const Perception& getPerception() const { return _perception; }

    /// @brief Mutable access — needed by TurnManager to call updateVisibility.
    Perception& getPerception() { return _perception; }

    /// @brief Reset this turn's visibility (call at start of each turn).
    void resetFogOfWar() { _perception.resetVisibility(); }

    /// @brief Recompute visibility from ships only (used by TurnManager).
    void refreshFogOfWar(const Map& map);

    /// @brief Recompute visibility from all civ entities — ships AND colonized planets.
    /// Use this overload from Game so that colonized planets always illuminate their sector.
    void refreshFogOfWar(const Map& map, const Civilization& civ);

private:
    int         _numericId;
    std::string _playerId;
    std::string _displayName;
    bool        _isAI;

    std::set<int> _ship_ids;  ///< Entity IDs owned by this player (no ownership)
    Perception    _perception;
};