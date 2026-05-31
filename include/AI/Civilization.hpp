#pragma once
#include "Utils/Updatable.hpp"
#include "Systems/Resource.hpp"
#include "Systems/Technology.hpp"
#include "Utils/Enums.hpp"
#include "Utils/Constants.hpp"
#include <unordered_map>
#include <set>
#include <string>

class Map;
class CombatSystem;
class Ship;

class Civilization : public Updatable {
protected:
    std::string _name;
    CivilizationType _type;
    GameUI::Color _color;
    Resource _resources;
    std::set<int> _entity_ids;
    std::unordered_map<const Civilization*, Relation> _relations;
    Technology _weapon_tech;  ///< Fighter attack-power research (ATTACK type)

public:
    Civilization(const std::string& name, CivilizationType type, Resource startingResources = Resource(100,100,100));
    virtual ~Civilization() = default;

    void update() override;
    void isAllyAttacked(const Civilization& ally, const Civilization* attacker);

    /// @brief Execute one AI turn for this civilization.
    /// Has no effect for PLAYER or PEACEFUL civilizations.
    /// Called by Game::advanceTurn() after all entity updates.
    void takeTurn(Map& map, CombatSystem& combat);

private:
    // ── AI helpers ────────────────────────────────────────────────────────────
    void aiActAggressive  (Ship& ship, Map& map, CombatSystem& combat);
    void aiActExpansionist(Ship& ship, Map& map, CombatSystem& combat);
    void aiActNeutral     (Ship& ship, Map& map, CombatSystem& combat);
    void aiRoam           (Ship& ship, Map& map);

public:

    // Entity fleet tracking
    void addEntity(int entity_id)    { _entity_ids.insert(entity_id); }
    void removeEntity(int entity_id) { _entity_ids.erase(entity_id);  }
    const std::set<int>& getEntityIDs() const { return _entity_ids;   }

    // Diplomacy
    Relation getRelationWith(const Civilization& other) const;
    void setRelationWith(const Civilization& other, Relation relation) { _relations[&other] = relation; }

    // Resources
    Resource&       getResources()       { return _resources; }
    const Resource& getResources() const { return _resources; }

    // Weapon technology (Fighter attack bonus)
    int       getAttackBonus()           const;
    bool      canUpgradeWeaponTech()     const;
    Resource  getWeaponTechUpgradeCost() const;
    void      upgradeWeaponTech();
    const Technology& getWeaponTech()    const { return _weapon_tech; }

    // Getters
    GameUI::Color    getColor()   const { return _color; }
    std::string      getName()    const { return _name;  }
    CivilizationType getCivType() const { return _type;  }

protected:
    virtual Relation defaultRelationWith(const Civilization& other) const;
};