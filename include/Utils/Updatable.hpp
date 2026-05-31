#pragma once

// Interface for any object that needs to be updated each turn.
// This allows different systems, such as entities and civilizations, to share the same update mechanism.
class Updatable{
public:
    virtual void update() = 0;
    virtual ~Updatable() = default;
};