#pragma once

#include "Systems/Resource.hpp"
#include "Utils/Enums.hpp"
#include <string>

/// @brief Represents a single researchable technology owned by a Civilization.
///
/// Technologies have levels from 0 (locked) up to maxLevel.
/// Each upgrade costs resources (not a flat int) so it integrates with
/// Civilization::getResources().canAfford().
///
/// Bonus values are type-dependent — a PRODUCTION tech gives no attack bonus.
/// Use the specific getter for the relevant tech type.
///
/// Civilizations hold a list of Technology objects. TurnManager dispatches
/// RESEARCH actions to the owning Civilization which calls upgrade() on the
/// correct Technology after verifying resources.
class Technology {
private:
    int        _id;
    std::string _name;
    TechType   _type;
    int        _level;
    int        _max_level;
    Resource   _base_cost;  ///< Cost to unlock (level 0 → 1); scales with level

public:
    /// @param id         Unique identifier within the civilization's tech tree
    /// @param name       Display name
    /// @param type       Determines which bonus this tech provides
    /// @param max_level  Cap on how many times this tech can be upgraded
    /// @param base_cost  Resource cost for the first unlock; scales linearly
    Technology(int id, const std::string& name, TechType type,
               int max_level, Resource base_cost);

    // ── Getters ───────────────────────────────────────────────────────────────

    int                getId()        const { return _id;        }
    const std::string& getName()      const { return _name;      }
    TechType           getType()      const { return _type;      }
    int                getLevel()     const { return _level;      }
    int                getMaxLevel()  const { return _max_level;  }
    bool               isUnlocked()   const { return _level > 0;  }
    bool               isMaxed()      const { return _level >= _max_level; }

    /// @brief Resource cost to upgrade from current level to next.
    /// Formula: base_cost * (current_level + 1)
    Resource getUpgradeCost() const;

    // ── Actions ───────────────────────────────────────────────────────────────

    /// @brief Returns true if this tech can still be upgraded.
    bool canUpgrade() const { return _level < _max_level; }

    /// @brief Increment level by 1. Does nothing if already at max level.
    /// Caller is responsible for deducting resources before calling this.
    void upgrade();

    // ── Bonus system (type-gated) ─────────────────────────────────────────────

    /// @brief Resource production multiplier bonus (PRODUCTION techs only).
    /// Returns 0 for non-PRODUCTION types.
    int getProductionBonus() const;

    /// @brief Defense/shield bonus applied to ships (DEFENSE techs only).
    /// Returns 0 for non-DEFENSE types.
    int getDefenseBonus() const;

    /// @brief Attack power bonus applied to ships (ATTACK techs only).
    /// Returns 0 for non-ATTACK types.
    int getAttackBonus() const;

    /// @brief Returns a formatted multi-line info string (matches codebase pattern).
    std::string getDetailedInfo() const;
};