#include "Player.hpp"
#include <iostream>
#include <iomanip>

void Player::printStatus() const {

    std::cout << "  Player [" << playerId_ << "] \n";
    std::cout << "  Name    : " << displayName_ << "\n";
    std::cout << "  AI      : " << (isAI_ ? "yes" : "no") << "\n";
    std::cout << "  Fleet   : " << aliveCount() << " alive / "
              << totalCount() << " total\n";
    std::cout << "  Defeated: " << (isDefeated() ? "yes" : "no") << "\n";
    std::cout << "  Explored: "
              << std::fixed << std::setprecision(1)
              << (perception_.explorationRatio() * 100.0f) << "%\n";
    std::cout << "  Visible cells this turn: "
              << perception_.visibleCount() << "\n";

    std::cout << "  Ships:\n";
    for (const auto& ship : fleet_) {
        std::cout << "    [" << ship->getId() << "] "
                  << (ship->isAlive() ? "alive" : "destroyed")
                  << "  pos=(" << ship->getX() << "," << ship->getY() << ")"
                  << "  vision=" << ship->getVisionRange() << "\n";
    }
}