#include "Player.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

void Player::printStatus() const {
    std::ostringstream buffer;

    buffer << "  Player [" << _playerId << "] \n";
    buffer << "  Name    : " << _displayName << "\n";
    buffer << "  AI      : " << (_isAI ? "yes" : "no") << "\n";
    buffer << "  Fleet   : " << aliveCount() << " alive / "
              << totalCount() << " total\n";
    buffer << "  Defeated: " << (isDefeated() ? "yes" : "no") << "\n";
    buffer << "  Explored: "
              << std::fixed << std::setprecision(1)
              << (_perception.explorationRatio() * 100.0f) << "%\n";
    buffer << "  Visible cells this turn: "
              << _perception.visibleCount() << "\n";

    buffer << "  Ships:\n";
    for (const auto& ship : _fleet) {
        buffer << "    [" << ship->getId() << "] "
                  << (ship->isAlive() ? "alive" : "destroyed")
                  << "  pos=(" << ship->getPosition().x << "," << ship->getPosition().y << ")"
                  << "  vision=" << ship->getVisionRange() << "\n";
    }
    std::cout << buffer.str();
}