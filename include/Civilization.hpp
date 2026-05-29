#pragma once
#include "Updatable.hpp"
#include "Resource.hpp"
#include "../src/Utils/Enums.hpp"
#include "../src/Utils/Constants.hpp"
#include <unordered_map>
#include <set>
#include <string>

class Civilization : public Updatable {
protected:
    std::string _name;
    CivilizationType _type;
    GameUI::Color _color;
    Resource _resources;
    std::set<int> _entity_ids;  ///< IDs of entities owned by this civilization (no ownership)
    std::unordered_map<const Civilization*, Relation> _relations;

public:
    Civilization(const std::string& name, CivilizationType type, Resource startingResources = Resource(100,100,100));
    virtual ~Civilization() = default;

    void update() override;
    void isAllyAttacked(const Civilization& ally, const Civilization* attacker);

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

    // Getters
    GameUI::Color    getColor()   const { return _color; }
    std::string      getName()    const { return _name;  }
    CivilizationType getCivType() const { return _type;  }

protected:
    virtual Relation defaultRelationWith(const Civilization& other) const;
};