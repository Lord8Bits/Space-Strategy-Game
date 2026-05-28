#include "../../include/Resource.hpp"
#include <algorithm> // std::max

// CONSTRUCTOR

Resource::Resource(int gold, int titanium, int cadmium)
    : _gold(gold), _titanium(titanium), _cadmium(cadmium) {}

// GETTERS

int Resource::getGold()     const { return _gold;     }
int Resource::getTitanium() const { return _titanium; }
int Resource::getCadmium()  const { return _cadmium;  }

// ADD FUNCTIONS (reject negative values)

void Resource::addGold(const int value)     { if (value > 0) _gold     += value; }
void Resource::addTitanium(const int value) { if (value > 0) _titanium += value; }
void Resource::addCadmium(const int value)  { if (value > 0) _cadmium  += value; }

// CONSUME FUNCTIONS (clamp to zero)

void Resource::consumeGold(const int value)     { if (_gold     >= value) _gold     -= value; }
void Resource::consumeTitanium(const int value) { if (_titanium >= value) _titanium -= value; }
void Resource::consumeCadmium(const int value)  { if (_cadmium  >= value) _cadmium  -= value; }

// BULK CHECK

bool Resource::canAfford(const Resource& cost) const
{
    return _gold >= cost._gold
        && _titanium >= cost._titanium
        && _cadmium  >= cost._cadmium;
}

// ARITHMETIC OPERATORS

Resource Resource::operator+(const Resource& other) const
{
    return Resource(_gold     + other._gold,
                    _titanium + other._titanium,
                    _cadmium  + other._cadmium);
}

Resource Resource::operator-(const Resource& other) const
{
    // Clamp each field to 0 — resources can't go negative via subtraction
    return Resource(std::max(0, _gold     - other._gold),
                    std::max(0, _titanium - other._titanium),
                    std::max(0, _cadmium  - other._cadmium));
}

Resource& Resource::operator+=(const Resource& other)
{
    _gold     += other._gold;
    _titanium += other._titanium;
    _cadmium  += other._cadmium;
    return *this;
}

Resource& Resource::operator-=(const Resource& other)
{
    _gold     = std::max(0, _gold     - other._gold);
    _titanium = std::max(0, _titanium - other._titanium);
    _cadmium  = std::max(0, _cadmium  - other._cadmium);
    return *this;
}

// COMPARISON OPERATORS

bool Resource::operator>=(const Resource& other) const { return canAfford(other); }

bool Resource::operator<(const Resource& other) const
{
    return !canAfford(other);
}

bool Resource::operator==(const Resource& other) const
{
    return _gold     == other._gold
        && _titanium == other._titanium
        && _cadmium  == other._cadmium;
}

bool Resource::operator!=(const Resource& other) const { return !(*this == other); }