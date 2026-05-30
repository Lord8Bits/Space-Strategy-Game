#include "../include/SubjectiveRender.hpp"
#include "../include/Map.hpp"
#include "../include/InputParser.hpp"
#include "../include/Player.hpp"
#include "../include/Civilization.hpp"
#include "../include/Fighter.hpp"
#include "../include/Cruiser.hpp"
#include "../include/Planet.hpp"
#include "../include/CombatSystem.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    Map map(2, 3);

    Civilization player_civ("Human Empire", CivilizationType::PLAYER, Resource(300, 200, 150));
    Civilization enemy_civ ("Aggressors",   CivilizationType::AGGRESSIVE);

    const int f1 = map.addEntity(std::make_unique<Fighter>("F-1", Vec2{10, 5},  &player_civ));
    const int c1 = map.addEntity(std::make_unique<Cruiser>("C-1", Vec2{30, 8},  &player_civ));
    const int e1 = map.addEntity(std::make_unique<Fighter>("E-1", Vec2{20, 10}, &enemy_civ));

    map.addEntity(std::make_unique<Planet>("Kepler", Vec2{50, 10},
                  PlanetType::MINERAL, Resource(500, 300, 200)));

    Player player(1, "P1", "Human Player");
    player.addShipId(f1);
    player.addShipId(c1);

    Render renderer;
    SubjectiveRender sr(renderer);
    CombatSystem combat;
    InputParser parser(map.getChunkRows() * map.getChunkCols());

    int turn = 1;
    std::string message = "Welcome, Commander. Issue your orders.";
    bool running = true;

    player.refreshFogOfWar(map);

    while (running) {
        sr.drawUI(player, player_civ, map, turn, message);
        message.clear();

        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        if (line == "quit") { running = false; continue; }

        if (line == "help") {
            std::ostringstream h;
            for (const auto& u : InputParser::getSupportedCommands()) h << u << "  |  ";
            message = h.str();
            continue;
        }

        if (line == "next") {
            for (int r = 0; r < map.getChunkRows(); ++r) {
                for (int c = 0; c < map.getChunkCols(); ++c) {
                    map.changeSelectedChunk(c, r);
                    std::vector<int> ids(map.getSelectedChunk().getEntityIDs().begin(),
                                        map.getSelectedChunk().getEntityIDs().end());
                    for (int id : ids) {
                        Entity* e = map.getEntity(id);
                        if (e) { e->update(); map.updateEntityChunk(id); }
                    }
                }
            }
            ++turn;
            player.refreshFogOfWar(map);
            message = "Turn advanced. Travelling ships moved closer to their destinations.";
            continue;
        }

        if (line.rfind("status", 0) == 0) {
            std::istringstream ss(line);
            std::string cmd; int id;
            ss >> cmd >> id;
            const Entity* e = map.getEntity(id);
            const Ship* s = dynamic_cast<const Ship*>(e);
            message = s ? s->getDetailedInfo() : "No ship with that id.";
            continue;
        }

        try {
            const Action a = parser.parseCommand(line);

            switch (a.action_type) {
                case Action::Type::VIEW_SECTOR: {
                    const int s = a.sector_id - 1;
                    map.changeSelectedChunk(s % map.getChunkCols(), s / map.getChunkCols());
                    message = "Now viewing sector " + std::to_string(a.sector_id) + ".";
                    break;
                }
                case Action::Type::MOVE: {
                    Entity* e = map.getEntity(a.entity_id);
                    Ship* s = dynamic_cast<Ship*>(e);
                    if (!s || !s->isAlive()) { message = "No controllable ship with that id."; break; }
                    s->setDestination(a.target_position);
                    s->advanceTowardDestination();
                    map.updateEntityChunk(a.entity_id);
                    player.refreshFogOfWar(map);
                    message = s->reachedDestination()
                        ? s->getName() + " arrived at destination."
                        : s->getName() + " moving... use 'next' to continue travel.";
                    break;
                }
                case Action::Type::ATTACK: {
                    Entity* atkE = map.getEntity(a.entity_id);
                    Entity* defE = map.getEntity(a.target_entity_id);
                    Ship* atk = dynamic_cast<Ship*>(atkE);
                    if (!atk || !atk->isAlive() || !defE) { message = "Invalid attacker or target."; break; }
                    combat.setCurrentTurn(turn);
                    const CombatResult res = atk->attack(*defE, combat);
                    std::ostringstream log;
                    for (const auto& l : res.logs) log << l.message << " | ";
                    if (res.isDestroyed) {
                        player.removeShipId(a.target_entity_id);
                        map.removeEntity(a.target_entity_id);
                    }
                    player.refreshFogOfWar(map);
                    message = log.str();
                    break;
                }
                default:
                    message = "That command isn't wired into the prototype yet.";
                    break;
            }
        }
        catch (const std::exception& ex) {
            message = InputParser::getErrorMessage(ex);
        }

        if (player.isDefeated()) {
            sr.drawUI(player, player_civ, map, turn, "All your ships destroyed. GAME OVER.");
            break;
        }
    }

    Render::clearScreen();
    std::cout << "Game ended after " << turn << " turns. Thanks for playing!\n";
    return 0;
}
