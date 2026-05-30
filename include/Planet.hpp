#pragma once

#include "Entity.hpp"
#include "Resource.hpp"
#include "../src/Utils/Enums.hpp"
#include <string>

/// @brief Represents a planet in the game world.
///
/// Planet is a concrete Entity subclass — it lives in Map like ships do,
/// gets indexed by Chunk, and is rendered by Render/SubjectiveRender.
///
/// Unlike ships, planets:
///   - Do not move
///   - Cannot be destroyed (isAlive always true)
///   - Hold a resource pool that Transports can mine/unload to
///   - Track colonization state and owning civilization
///
/// PlanetType determines the resource yield profile (see Enums.hpp).
class Planet : public Entity {
private:
    PlanetType _planet_type;
    Resource   _resources;
    bool       _colonized;

public:
    /// @param name        Display name of the planet
    /// @param pos         World position (fixed — planets don't move)
    /// @param type        Resource profile of this planet
    /// @param resources   Starting resource pool available for mining
    /// @param owner       Owning civilization (nullptr = unclaimed)
    Planet(const std::string& name, const Vec2& pos, PlanetType type,
           Resource resources, Civilization* owner = nullptr);

    // ── Entity interface ──────────────────────────────────────────────────────

    /// @brief All planets render as ● ; color encodes type (green/yellow/cyan).
    std::string getSymbol()      const override { return "\xE2\x97\x8F"; } // ●
    GameUI::Color getDisplayColor() const override;
    EntityType getType()   const override { return EntityType::PLANET; }
    std::string getDetailedInfo() const override;
    bool isAlive()          const override { return true; }   ///< Planets cannot be destroyed
    void takeDamage(int)          override {}                 ///< Planets ignore damage
    int  getXpReward()      const override { return 0; }      ///< No XP for attacking a planet
    bool canBeAttacked()    const override { return false; }  ///< Planets cannot be targeted
    void update()                override {}

    /// @brief Planets do not initiate combat — no-op.
    void interactEntity(Entity* other, CombatSystem& combatSystem) override {}

    // ── Planet-specific interface ─────────────────────────────────────────────

    /// @brief Returns true if the planet still has at least one resource available.
    bool hasResources() const { return _resources.total() > 0; }

    /// @brief Returns true if this planet has been claimed by a civilization.
    bool isColonized() const { return _colonized; }

    /// @brief Claim this planet for the given civilization.
    /// Sets owner and marks as colonized.
    void colonize(Civilization* new_owner);

    /// @brief Attempt to extract resources from this planet.
    /// @param requested  The amount requested by a Transport
    /// @return           The amount actually extracted (capped by available resources)
    Resource extract(const Resource& requested);

    // ── Getters ───────────────────────────────────────────────────────────────
    PlanetType       getPlanetType() const { return _planet_type;  }
    const Resource&  getResources()  const { return _resources;    }
};