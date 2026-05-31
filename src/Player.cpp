#include "../include/Player.hpp"
#include "../include/Map.hpp"
#include "../include/Civilization.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

void Player::refreshFogOfWar(const Map& map) {
    resetFogOfWar();
    for (const int id : _ship_ids) {
        const Entity* e = map.getEntity(id);
        if (!e || !e->isAlive()) continue;
        _perception.updateVisibility(
            e->getPosition().x, e->getPosition().y,
            e->getVisionRange(), map.getWorldWidth(), map.getWorldHeight());
    }
}

void Player::refreshFogOfWar(const Map& map, const Civilization& civ) {
    resetFogOfWar();
    for (const int id : civ.getEntityIDs()) {
        const Entity* e = map.getEntity(id);
        if (!e || !e->isAlive()) continue;
        const int vr = e->getVisionRange();
        if (vr <= 0) continue;
        _perception.updateVisibility(
            e->getPosition().x, e->getPosition().y,
            vr, map.getWorldWidth(), map.getWorldHeight());
    }
}

void Player::printStatus(const Map& map) const {
    std::ostringstream buffer;

    buffer << "Player [" << _playerId << "]\n";
    buffer << "  Name    : " << _displayName << "\n";
    buffer << "  AI      : " << (_isAI ? "yes" : "no") << "\n";
    buffer << "  Fleet   : " << _ship_ids.size() << " registered\n";
    buffer << "  Defeated: " << (isDefeated() ? "yes" : "no") << "\n";
    buffer << "  Explored: "
           << std::fixed << std::setprecision(1)
           << (_perception.explorationRatio() * 100.0f) << "%\n";
    buffer << "  Visible cells this turn: " << _perception.visibleCount() << "\n";

    buffer << "  Ships:\n";
    for (const int id : _ship_ids) {
        const Entity* e = map.getEntity(id);
        if (!e) {
            buffer << "    [" << id << "] (not found in map)\n";
            continue;
        }
        buffer << "    [" << id << "] "
               << e->getName()
               << "  pos=(" << e->getPosition().x << "," << e->getPosition().y << ")\n";
    }

    std::cout << buffer.str();
}