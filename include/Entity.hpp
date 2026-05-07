#pragma once
#include <string>
#include "../src/Utils/Position.hpp"
#include "../src/Utils/Enums.hpp"
#include "Civilization.hpp"

class Entity{
protected:
    static int _next_id;
    const int _id;
    std::string name;
    Position position;
    Civilization* owner;

public:
    Entity(const std::string& Name, const Position& Pos, Civilization* Owner = nullptr);
    virtual ~Entity() = default;    
    
    //Pure virtual functions
    virtual void update() = 0;
    virtual void render() const;
    virtual char getSymbol() const = 0;
    virtual EntityType getType() const = 0;
    virtual void interactEntity(Entity* other) = 0;
    virtual std::string getDetailedInfo() const = 0;

    int getId() const { return id; }
    std::string getName() const { return name; }
    Position getPosition() const { return position; }
    void setPosition(const Position& pos) { position = pos; }
    Civilization* getCivOwner() const { return owner; }
    void setCivOwner(Civilization* newOwner) { owner = newOwner; }
};