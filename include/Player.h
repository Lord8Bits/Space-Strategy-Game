#pragma once

#include "Ship.h"
#include "Perception.h"

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
        : numericId_(numericId)
        , playerId_(std::move(playerId))
        , displayName_(std::move(displayName))
        , isAI_(isAI)
        , perception_()
    {}

    Player(const Player&)            = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&)                 = default;
    Player& operator=(Player&&)      = default;


    
    //  Player info


    /// @brief Returns the numeric ID of the player
    int getNumericId() const { return numericId_; }

    /// @brief Returns the string ID of the player (ex: "P1", "CPU")
    const std::string& getId() const { return playerId_; }

    /// @brief Returns the display name shown in the UI
    const std::string& getDisplayName() const { return displayName_; }

    /// @brief Returns true if the player is controlled by AI
    bool isAI() const { return isAI_; }

    /// @brief Returns true if the player is human
    bool isHuman() const { return !isAI_; }

    
    
    //  Fleet management
 

    /// @brief Adds a ship to the fleet. Throws if ship is null or ID already exists.
    void addShip(std::unique_ptr<Ship> ship) {
        if (!ship)
            throw std::invalid_argument("Null ship.");
        const std::string& sid = ship->getId();
        if (shipIndex_.count(sid))
            throw std::invalid_argument("Ship id already exists: " + sid);
        shipIndex_[sid] = ship.get();
        fleet_.push_back(std::move(ship));
    }

    /// @brief Removes a ship by ID. Returns true if found and removed.
    bool removeShip(const std::string& shipId) {
        auto it = shipIndex_.find(shipId);
        if (it == shipIndex_.end()) return false;
        shipIndex_.erase(it);
        fleet_.erase(
            std::remove_if(fleet_.begin(), fleet_.end(),
                [&](const std::unique_ptr<Ship>& s) {
                    return s->getId() == shipId;
                }),
            fleet_.end()
        );
        return true;
    }

    /// @brief Returns a pointer to a ship by ID, or nullptr if not found
    Ship* getShip(const std::string& shipId) const {
        auto it = shipIndex_.find(shipId);
        return (it != shipIndex_.end()) ? it->second : nullptr;
    }

    /// @brief Returns a pointer to a ship by index, or nullptr if out of range
    Ship* getShipAt(std::size_t index) const {
        if (index >= fleet_.size()) return nullptr;
        return fleet_[index].get();
    }

    /// @brief Returns the full fleet (read-only)
    const std::vector<std::unique_ptr<Ship>>& getFleet() const {
        return fleet_;
    }

    
    //  Fleet status


    /// @brief Returns the number of ships still alive
    int aliveCount() const {
        int count = 0;
        for (const auto& ship : fleet_)
            if (ship->isAlive()) ++count;
        return count;
    }

    /// @brief Returns the total number of ships including destroyed ones
    int totalCount() const { return static_cast<int>(fleet_.size()); }

    /// @brief Returns true if all ships are destroyed
    bool isDefeated() const { return aliveCount() == 0; }

    /// @brief Returns true if at least one ship is alive
    bool hasShipsAlive() const { return aliveCount() > 0; }

    /// @brief Removes all destroyed ships from the fleet and the index
    void purgeDestroyedShips() {
        fleet_.erase(
            std::remove_if(fleet_.begin(), fleet_.end(),
                [&](const std::unique_ptr<Ship>& ship) {
                    if (!ship->isAlive()) {
                        shipIndex_.erase(ship->getId());
                        return true;
                    }
                    return false;
                }),
            fleet_.end()
        );
    }


    //  Perception / Fog of War

    /// @brief Returns a reference to the player's Perception object
    Perception&       getPerception()       { return perception_; }
    const Perception& getPerception() const { return perception_; }

    /// @brief Updates fog of war from all alive ships' positions and vision ranges
    void updateFogOfWar() {
        perception_.resetVisibility();
        for (const auto& ship : fleet_) {
            if (!ship->isAlive()) continue;
            perception_.updateVisibility(
                ship->getX(),
                ship->getY(),
                ship->getVisionRange()
            );
        }
    }

    void printStatus() const;

private:

    int         numericId_;
    std::string playerId_;
    std::string displayName_;
    bool        isAI_;

    std::vector<std::unique_ptr<Ship>>    fleet_;
    std::unordered_map<std::string, Ship*> shipIndex_;

    Perception perception_;
};