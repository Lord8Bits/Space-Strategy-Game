#ifndef RESOURCE_H
#define RESOURCE_H

#include <ostream>

class Resource {

private:
    int _gold;
    int _titanium;
    int _cadmium;

public:
    Resource(int gold, int titanium, int cadmium);

    // GETTERS
    int getGold()     const;
    int getTitanium() const;
    int getCadmium()  const;
    int total()       const { return _gold + _titanium + _cadmium; }

    // ADD / CONSUME (per-resource, guarded)
    void addGold(int value);
    void addTitanium(int value);
    void addCadmium(int value);

    void consumeGold(int value);
    void consumeTitanium(int value);
    void consumeCadmium(int value);

    // BULK CHECK: does this resource pool cover a given cost?
    bool canAfford(const Resource& cost) const;

    // ARITHMETIC OPERATORS (combine two Resource objects)
    Resource  operator+ (const Resource& other) const;
    Resource  operator- (const Resource& other) const;
    Resource& operator+=(const Resource& other);
    Resource& operator-=(const Resource& other);

    // COMPARISON OPERATORS
    bool operator>=(const Resource& other) const;

    bool operator<(const Resource &other) const;

    bool operator==(const Resource& other) const;
    bool operator!=(const Resource& other) const;

    /// @brief Stream output — friend so it can read private fields directly.
    /// Example: std::cout << myResource;
    friend std::ostream& operator<<(std::ostream& os, const Resource& r);
};

#endif