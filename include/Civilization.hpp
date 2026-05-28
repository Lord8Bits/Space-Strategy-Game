#pragma once
#include "Updatable.hpp"
#include "../src/Utils/Enums.hpp"
#include "../src/Utils/Constants.hpp"
#include <unordered_map>
#include <string>

//Forward declaration & initialization
class Civilization : public Updatable{
protected:
    std::string _name;
    CivilizationType type;
    GameUI::Color color;
    std::unordered_map<const Civilization*, Relation> _relations;

public:
    Civilization(CivilizationType Type);
    virtual ~Civilization() = default;

    void update() override;
    void isAllyAttacked(const Civilization& ally, const Civilization* attacker);

    GameUI::Color getColor() const { return color; }

    std::string getName() const { return _name; }
    CivilizationType getCivType() const { return type; }
    Relation getRelationWith(const Civilization& other) const;
    void setRelationWith(const Civilization& other, Relation relation) { _relations[&other] = relation; }

protected:
    virtual Relation defaultRelationWith(const Civilization& other) const;
};