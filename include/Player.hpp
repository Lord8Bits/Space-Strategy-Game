#pragma once

#include "Ship.hpp"
#include "Perception.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

class Player {
public:

    Player(
        int numericId,
        std::string playerId,
        std::string displayName,
        bool isAI = false
    )
        : _numericId(numericId)
        , _playerId(std::move(playerId))
        , _displayName(std::move(displayName))
        , _isAI(isAI)
        , _perception()
    {}

    Player(const Player&)            = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&)                 = default;
    Player& operator=(Player&&)      = default;


    
    //  Player info


    /// @brief Returns the numeric ID of the player
    int getNumericId() const { return _numericId; }

    /// @brief Returns the string ID of the player (ex: "P1", "CPU")
    const std::string& getId() const { return _playerId; }

    /// @brief Returns the display name shown in the UI
    const std::string& getDisplayName() const { return _displayName; }

    /// @brief Returns true if the player is controlled by AI
    bool isAI() const { return _isAI; }

    /// @brief Returns true if the player is human
    bool isHuman() const { return !_isAI; }

    
    
    //  Fleet management
 

    /// @brief Adds a ship to the fleet. Throws if ship is null or ID already exists.
    void addShip(std::unique_ptr<Ship> ship) {
        if (!ship)
            throw std::invalid_argument("Null ship.");
        const int shipId = ship->getId();
        if (_shipIndex.count(shipId))
            throw std::invalid_argument("Ship id already exists: " + std::to_string(shipId));
        _shipIndex[shipId] = ship.get();
        _fleet.push_back(std::move(ship));
    }

    /// @brief Removes a ship by ID. Returns true if found and removed.
    bool removeShip(int shipId) {
        auto it = _shipIndex.find(shipId);
        if (it == _shipIndex.end()) return false;
        _shipIndex.erase(it);
        _fleet.erase(
            std::remove_if(_fleet.begin(), _fleet.end(),
                [&](const std::unique_ptr<Ship>& s) {
                    return s->getId() == shipId;
                }),
            _fleet.end()
        );
        return true;
    }

    /// @brief Returns a pointer to a ship by ID, or nullptr if not found
    Ship* getShip(int shipId) const {
        auto it = _shipIndex.find(shipId);
        return (it != _shipIndex.end()) ? it->second : nullptr;
    }

    /// @brief Returns a pointer to a ship by index, or nullptr if out of range
    Ship* getShipAt(std::size_t index) const {
        if (index >= _fleet.size()) return nullptr;
        return _fleet[index].get();
    }

    /// @brief Returns the full fleet (read-only)
    const std::vector<std::unique_ptr<Ship>>& getFleet() const {
        return _fleet;
    }

    
    //  Fleet status


    /// @brief Returns the number of ships still alive
    int aliveCount() const {
        int count = 0;
        for (const auto& ship : _fleet)
            if (ship->isAlive()) ++count;
        return count;
    }

    /// @brief Returns the total number of ships including those pending purge
    int totalCount() const { return static_cast<int>(_fleet.size()); }

    /// @brief Returns true if all ships are destroyed
    bool isDefeated() const { return aliveCount() == 0; }

    /// @brief Returns true if at least one ship is alive
    bool hasShipsAlive() const { return aliveCount() > 0; }

    /// @brief Removes all destroyed ships from the fleet and the index
    void purgeDestroyedShips() {
        _fleet.erase(
            std::remove_if(_fleet.begin(), _fleet.end(),
                [&](const std::unique_ptr<Ship>& ship) {
                    if (!ship->isAlive()) {
                        _shipIndex.erase(ship->getId());
                        return true;
                    }
                    return false;
                }),
            _fleet.end()
        );
    }


    //  Perception / Fog of War

    /// @brief Returns a reference to the player's Perception object
    const Perception& getPerception() const { return _perception; }

    /// @brief Updates fog of war from all alive ships' positions and vision ranges
    void updateFogOfWar() {
        _perception.resetVisibility();
        for (const auto& ship : _fleet) {
            if (!ship->isAlive()) continue;
            _perception.updateVisibility(
                ship->getPosition().x,
                ship->getPosition().y,
                ship->getVisionRange()
            );
        }
    }

    void printStatus() const;

private:

    const int _numericId;
    std::string _playerId;
    std::string _displayName;
    bool _isAI;

    std::vector<std::unique_ptr<Ship>> _fleet;
    std::unordered_map<int, Ship*> _shipIndex;

    Perception _perception;
};
