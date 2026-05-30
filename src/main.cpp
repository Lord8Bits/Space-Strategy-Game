#include "../include/SubjectiveRender.hpp"
#include "../include/Map.hpp"
#include "../include/InputParser.hpp"
#include "../include/Player.hpp"
#include "../include/Civilization.hpp"
#include "../include/Fighter.hpp"
#include "../include/Cruiser.hpp"
#include "../include/Transport.hpp"
#include "../include/Planet.hpp"
#include "../include/CombatSystem.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    // 2 rows × 3 cols = 6 sectors, each 80×20 → world 240×40
    Map map(2, 3);

    Civilization player_civ("Human Empire", CivilizationType::PLAYER,   Resource(300, 200, 150));
    Civilization enemy_civ ("Aggressors",   CivilizationType::AGGRESSIVE);

    // ── Player fleet ──────────────────────────────────────────────────────────
    const int f1 = map.addEntity(std::make_unique<Fighter>  ("F-1", Vec2{15,  5}, &player_civ));
    const int c1 = map.addEntity(std::make_unique<Cruiser>  ("C-1", Vec2{25,  8}, &player_civ));
    const int t1 = map.addEntity(std::make_unique<Transport>("T-1", Vec2{20, 14}, &player_civ));

    // ── Enemy fleet (sector 2, second chunk column) ───────────────────────────
    const int e1 = map.addEntity(std::make_unique<Fighter>  ("E-1", Vec2{90, 5},  &enemy_civ));
    const int e2 = map.addEntity(std::make_unique<Cruiser>  ("E-2", Vec2{100, 10}, &enemy_civ));

    // ── Home planet (player, sector 1) ────────────────────────────────────────
    const int home_id = map.addEntity(std::make_unique<Planet>(
        "Terra", Vec2{40, 10}, PlanetType::TERRAIN, Resource(400, 300, 200), &player_civ));

    // ── Other planets ─────────────────────────────────────────────────────────
    map.addEntity(std::make_unique<Planet>("Kepler",  Vec2{130, 8},  PlanetType::MINERAL, Resource(600, 400, 100)));
    map.addEntity(std::make_unique<Planet>("Solaris", Vec2{160, 15}, PlanetType::ENERGY,  Resource(200, 100, 500)));

    // ── Player setup ──────────────────────────────────────────────────────────
    Player player(1, "P1", "Human Player");
    player.addShipId(f1);
    player.addShipId(c1);
    player.addShipId(t1);

    // Register entities with their civilization
    player_civ.addEntity(f1);
    player_civ.addEntity(c1);
    player_civ.addEntity(t1);
    player_civ.addEntity(home_id);
    enemy_civ.addEntity(e1);
    enemy_civ.addEntity(e2);

    Render        renderer;
    SubjectiveRender sr(renderer);
    CombatSystem  combat;
    InputParser   parser(map.getChunkRows() * map.getChunkCols());

    int  turn       = 1;
    int  ship_count = 3;   // tracks how many ships built so far for naming
    std::string message = "Welcome, Commander. Your home world Terra is at (40,10). Issue your orders.";
    bool running = true;

    player.refreshFogOfWar(map);

    while (running) {
        sr.drawUI(player, player_civ, map, turn, message);
        message.clear();

        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        // ── Meta commands ─────────────────────────────────────────────────────
        if (line == "quit") { running = false; continue; }

        if (line == "help") {
            std::ostringstream h;
            for (const auto& u : InputParser::getSupportedCommands()) h << "  " << u << "\n";
            message = h.str();
            continue;
        }

        if (line == "next") {
            const int saved = map.getSelectedChunkIndex();
            for (int r = 0; r < map.getChunkRows(); ++r) {
                for (int c = 0; c < map.getChunkCols(); ++c) {
                    map.changeSelectedChunk(c, r);
                    // Copy IDs — update() can shift a ship into a different chunk
                    std::vector<int> ids(map.getSelectedChunk().getEntityIDs().begin(),
                                        map.getSelectedChunk().getEntityIDs().end());
                    for (int id : ids) {
                        Entity* e = map.getEntity(id);
                        if (e) { e->update(); map.updateEntityChunk(id); }
                    }
                }
            }
            // Restore the sector the player was viewing
            map.changeSelectedChunk(saved % map.getChunkCols(), saved / map.getChunkCols());
            ++turn;
            player.refreshFogOfWar(map);
            message = "Turn " + std::to_string(turn) + " started.";
            continue;
        }

        if (line.rfind("status", 0) == 0) {
            std::istringstream ss(line);
            std::string cmd; int id;
            ss >> cmd >> id;
            const Entity* e = map.getEntity(id);
            if (!e) { message = "No entity with id " + std::to_string(id) + "."; continue; }
            message = e->getDetailedInfo();
            continue;
        }

        // ── Game commands ─────────────────────────────────────────────────────
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
                    Ship*   s = dynamic_cast<Ship*>(e);
                    if (!s || !s->isAlive()) { message = "No controllable ship with that id."; break; }
                    s->setDestination(a.target_position);
                    s->advanceTowardDestination();
                    map.updateEntityChunk(a.entity_id);
                    player.refreshFogOfWar(map);
                    message = s->reachedDestination()
                        ? s->getName() + " arrived at (" + std::to_string(a.target_position.x) + "," + std::to_string(a.target_position.y) + ")."
                        : s->getName() + " moving toward (" + std::to_string(a.target_position.x) + "," + std::to_string(a.target_position.y) + "). Use 'next' to continue.";
                    break;
                }

                case Action::Type::ATTACK: {
                    Entity* atkE = map.getEntity(a.entity_id);
                    Entity* defE = map.getEntity(a.target_entity_id);
                    Ship*   atk  = dynamic_cast<Ship*>(atkE);
                    if (!atk || !atk->isAlive() || !defE) { message = "Invalid attacker or target."; break; }
                    combat.setCurrentTurn(turn);
                    const CombatResult res = atk->attack(*defE, combat);
                    std::ostringstream log;
                    for (const auto& l : res.logs) log << l.message << " | ";
                    if (res.isDestroyed) {
                        player.removeShipId(a.target_entity_id);
                        enemy_civ.removeEntity(a.target_entity_id);
                        map.removeEntity(a.target_entity_id);
                    }
                    player.refreshFogOfWar(map);
                    message = log.str();
                    break;
                }

                case Action::Type::BUILD: {
                    Entity* planetE = map.getEntity(a.entity_id);
                    Planet* planet  = dynamic_cast<Planet*>(planetE);
                    if (!planet || !planet->isColonized() || planet->getCivOwner() != &player_civ) {
                        message = "BUILD requires one of your colonized planets.";
                        break;
                    }

                    // Determine type, name, and cost
                    const Vec2 spawn = planet->getPosition();
                    Resource cost(0, 0, 0);
                    std::unique_ptr<Ship> new_ship;
                    std::string type_name;
                    ++ship_count;

                    if (a.value == 0) {
                        cost = Resource(GameConstants::FIGHTER_BUILD_GOLD,
                                        GameConstants::FIGHTER_BUILD_TITANIUM,
                                        GameConstants::FIGHTER_BUILD_CADMIUM);
                        type_name = "Fighter";
                        new_ship  = std::make_unique<Fighter>("F-" + std::to_string(ship_count), spawn, &player_civ);
                    } else if (a.value == 1) {
                        cost = Resource(GameConstants::CRUISER_BUILD_GOLD,
                                        GameConstants::CRUISER_BUILD_TITANIUM,
                                        GameConstants::CRUISER_BUILD_CADMIUM);
                        type_name = "Cruiser";
                        new_ship  = std::make_unique<Cruiser>("C-" + std::to_string(ship_count), spawn, &player_civ);
                    } else {
                        cost = Resource(GameConstants::TRANSPORT_BUILD_GOLD,
                                        GameConstants::TRANSPORT_BUILD_TITANIUM,
                                        GameConstants::TRANSPORT_BUILD_CADMIUM);
                        type_name = "Transport";
                        new_ship  = std::make_unique<Transport>("T-" + std::to_string(ship_count), spawn, &player_civ);
                    }

                    if (!player_civ.getResources().canAfford(cost)) {
                        message = "Not enough resources to build a " + type_name
                            + ". Need Gold:" + std::to_string(cost.getGold())
                            + " Titanium:" + std::to_string(cost.getTitanium())
                            + " Cadmium:" + std::to_string(cost.getCadmium()) + ".";
                        --ship_count;  // roll back counter
                        break;
                    }

                    player_civ.getResources() -= cost;
                    const int new_id = map.addEntity(std::move(new_ship));
                    player.addShipId(new_id);
                    player_civ.addEntity(new_id);
                    player.refreshFogOfWar(map);
                    message = type_name + " [" + std::to_string(new_id) + "] built at "
                            + planet->getName() + ".";
                    break;
                }

                default:
                    message = "That command isn't wired in yet.";
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
