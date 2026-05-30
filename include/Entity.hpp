#pragma once
#include <string>
#include "../src/Utils/Position.hpp"
#include "../src/Utils/Enums.hpp"
#include "Civilization.hpp"

class CombatSystem;
class Ship;

class Entity : public Updatable{
protected:
    static int _next_id; //Unique id for each entity
    int _id;
    std::string _name;
    Vec2 _position;
    Civilization* _owner;

public:
    Entity(const std::string& Name, const Vec2& Pos, Civilization* Owner = nullptr);
    virtual ~Entity() = default;

    //Pure virtual functions
    virtual std::string getSymbol() const = 0;
    virtual EntityType getType() const = 0;
    virtual void interactEntity(Entity* other, CombatSystem& combatSystem) = 0;
    virtual std::string getDetailedInfo() const = 0;
    virtual bool isAlive() const = 0;
    virtual void takeDamage(int damage) = 0;
    virtual int getXpReward() const = 0;

    // Combat participation — override to opt out (e.g. Planet) or add special behaviour.
    virtual bool canBeAttacked() const { return true; }
    virtual bool tryDodge(const Ship& attacker) const { return false; }
    virtual int  absorbDamage(int damage) { return damage; }

    // Vision — used by FoW update; non-mobile entities return 0.
    virtual int getVisionRange() const { return 0; }

    // Display color — defaults to owner civ color; entities override for type-based coloring.
    virtual GameUI::Color getDisplayColor() const {
        return _owner ? _owner->getColor() : GameUI::Color::WHITE;
    }

    int getId() const { return _id; }
    std::string getName() const { return _name; }
    Vec2 getPosition() const { return _position; }
    void setPosition(const Vec2& pos) { _position = pos; }
    Civilization* getCivOwner() const { return _owner; }
    void setCivOwner(Civilization* newOwner) { _owner = newOwner; }
};
