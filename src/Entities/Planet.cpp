#include "../../include/Planet.hpp"
#include <sstream>

Planet::Planet(const std::string& name, const Vec2& pos, PlanetType type,
               Resource resources, Civilization* owner)
    : Entity(name, pos, owner)
    , _planet_type(type)
    , _resources(resources)
    , _colonized(owner != nullptr)  // pre-colonized if an owner is provided
{}

char Planet::getSymbol() const {
    switch (_planet_type) {
        case PlanetType::TERRAIN : return 'P';
        case PlanetType::MINERAL : return 'M';
        case PlanetType::ENERGY  : return 'E';
        default                  : return '?';
    }
}

void Planet::colonize(Civilization* new_owner) {
    _owner     = new_owner;
    _colonized = (new_owner != nullptr);
}

Resource Planet::extract(const Resource& requested) {
    // Only extract what the planet actually has
    const Resource available(
        std::min(requested.getGold(),     _resources.getGold()),
        std::min(requested.getTitanium(), _resources.getTitanium()),
        std::min(requested.getCadmium(),  _resources.getCadmium())
    );
    _resources -= available;
    return available;
}

std::string Planet::getDetailedInfo() const {
    std::stringstream ss;
    ss << "=== " << _name << " ===\n";
    ss << "Type: ";
    switch (_planet_type) {
        case PlanetType::TERRAIN : ss << "Terrain";  break;
        case PlanetType::MINERAL : ss << "Mineral";  break;
        case PlanetType::ENERGY  : ss << "Energy";   break;
    }
    ss << "\n";
    ss << "Position  : (" << _position.x << ", " << _position.y << ")\n";
    ss << "Colonized : " << (_colonized ? "yes" : "no") << "\n";
    ss << "Owner     : " << (_owner ? _owner->getName() : "none") << "\n";
    ss << "Resources : " << _resources.total() << " total\n";
    ss << "  Gold     : " << _resources.getGold()     << "\n";
    ss << "  Titanium : " << _resources.getTitanium() << "\n";
    ss << "  Cadmium  : " << _resources.getCadmium()  << "\n";
    return ss.str();
}