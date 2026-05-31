#ifndef RESOURCE_H
#define RESOURCE_H

#include <ostream>

/// @brief Represents a bundle of three resource types: gold, titanium, and cadmium.
///
/// Resource is used both as a currency (build costs, research costs) and as an
/// inventory (planet resource pools, Transport cargo, civilization treasury).
///
/// Operator overloads allow natural arithmetic on resource bundles:
///   treasury += mined;     // add extracted resources to the civ treasury
///   treasury -= build_cost; // deduct ship construction cost
///   if (treasury >= cost)   // check affordability before spending
///
/// Friend function:
///   operator<< is declared friend so it can read _gold, _titanium, _cadmium
///   directly without public getters — demonstrating the friend concept.
class Resource {

private:
    int _gold;      ///< General-purpose currency used for most build costs
    int _titanium;  ///< Heavy metal required for warship construction
    int _cadmium;   ///< Energy mineral required for advanced upgrades

public:
    /// @param gold     Starting gold amount
    /// @param titanium Starting titanium amount
    /// @param cadmium  Starting cadmium amount
    Resource(int gold, int titanium, int cadmium);

    // ── Getters ───────────────────────────────────────────────────────────────
    int getGold()     const;
    int getTitanium() const;
    int getCadmium()  const;

    /// @brief Sum of all three resource types.
    int total() const { return _gold + _titanium + _cadmium; }

    // ── Per-resource mutation (guarded against going negative) ────────────────
    void addGold(int value);
    void addTitanium(int value);
    void addCadmium(int value);

    void consumeGold(int value);
    void consumeTitanium(int value);
    void consumeCadmium(int value);

    /// @brief Returns true if every field in this pool is >= the matching field in cost.
    bool canAfford(const Resource& cost) const;

    // ── Arithmetic operator overloads ─────────────────────────────────────────

    /// @brief Component-wise addition — returns a new Resource (does not modify).
    Resource  operator+ (const Resource& other) const;

    /// @brief Component-wise subtraction — clamps each field to zero (no negative resources).
    Resource  operator- (const Resource& other) const;

    /// @brief Add another resource bundle in-place (e.g. treasury += mined).
    Resource& operator+=(const Resource& other);

    /// @brief Subtract another resource bundle in-place (e.g. treasury -= cost).
    Resource& operator-=(const Resource& other);

    // ── Comparison operator overloads ─────────────────────────────────────────

    /// @brief True if every field in this >= matching field in other (affordability check).
    bool operator>=(const Resource& other) const;

    /// @brief True if every field in this < matching field in other.
    bool operator< (const Resource& other) const;

    bool operator==(const Resource& other) const;
    bool operator!=(const Resource& other) const;

    // ── Friend stream output ──────────────────────────────────────────────────

    /// @brief Prints "Gold:X Titanium:Y Cadmium:Z" directly accessing private fields.
    /// Declared friend to demonstrate the friend function concept (accesses _gold,
    /// _titanium, _cadmium without going through public getters).
    friend std::ostream& operator<<(std::ostream& os, const Resource& r);
};

#endif