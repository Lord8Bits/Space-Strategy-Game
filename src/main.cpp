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

// ─── Helpers ──────────────────────────────────────────────────────────────────

static void clearScreen() { std::cout << "\033[2J\033[3J\033[H"; }

static void refreshFogOfWar(Player& player, const Map& map) {
    player.resetFogOfWar();
    for (const int id : player.getShipIds()) {
        const Entity* e = map.getEntity(id);
        if (!e) continue;
        const Ship* ship = dynamic_cast<const Ship*>(e);
        if (!ship || !ship->isAlive()) continue;
        player.getPerception().updateVisibility(
            ship->getPosition().x, ship->getPosition().y,
            ship->getVisionRange(), map.getWorldWidth(), map.getWorldHeight());
    }
}

/// @brief Which sector (1-based) currently contains a world position.
static int sectorOf(const Map& map, const Vec2& pos) {
    const int col = pos.x / VIEWPORT_WIDTH;
    const int row = pos.y / VIEWPORT_HEIGHT;
    return row * map.getChunkCols() + col + 1;
}

/// @brief Draw the full UI: map + fleet panel + resources + sector info + prompt.
static void drawUI(SubjectiveRender& sr, const Player& player, Civilization& civ,
                   Map& map, int turn, const std::string& last_message) {
    clearScreen();

    // --- World viewport (FoW filtered) ---
    sr.draw(player, map);

    const int selected = map.getSelectedChunkIndex() + 1;
    const Resource& res = civ.getResources();

    // --- Status panel ---
    std::ostringstream ui;
    ui << "================================================================\n";
    ui << " Turn " << turn
       << "  |  Civ: " << civ.getName()
       << "  |  Viewing Sector " << selected << "/" << (map.getChunkRows()*map.getChunkCols())
       << "  |  Explored " << static_cast<int>(player.getPerception().explorationRatio()*100) << "%\n";
    ui << " Resources -> Gold: " << res.getGold()
       << "   Titanium: " << res.getTitanium()
       << "   Cadmium: " << res.getCadmium() << "\n";
    ui << "----------------------------------------------------------------\n";
    ui << " YOUR FLEET:\n";
    for (const int id : player.getShipIds()) {
        const Entity* e = map.getEntity(id);
        if (!e) continue;
        const Ship* s = dynamic_cast<const Ship*>(e);
        if (!s) continue;
        ui << "   " << s->getDetailedInfo()
           << "  (sector " << sectorOf(map, s->getPosition()) << ")\n";
    }
    ui << "----------------------------------------------------------------\n";

    // --- Visible planets ---
    bool anyPlanet = false;
    for (int r = 0; r < map.getChunkRows(); ++r) {
        for (int c = 0; c < map.getChunkCols(); ++c) {
            map.changeSelectedChunk(c, r);
            for (const int id : map.getSelectedChunk().getEntityIDs()) {
                const Entity* e = map.getEntity(id);
                const Planet* p = dynamic_cast<const Planet*>(e);
                if (!p) continue;
                const Vec2 pp = p->getPosition();
                if (!player.getPerception().isDiscovered(pp.x, pp.y, map.getWorldWidth())) continue;
                if (!anyPlanet) { ui << " KNOWN PLANETS:\n"; anyPlanet = true; }
                ui << "   " << p->getName()
                   << "  pos(" << pp.x << "," << pp.y << ")"
                   << "  resources " << p->getResources().total()
                   << "  (sector " << sectorOf(map, pp) << ")\n";
            }
        }
    }
    // restore the viewed chunk index
    {
        const int sel = selected - 1;
        map.changeSelectedChunk(sel % map.getChunkCols(), sel / map.getChunkCols());
    }
    if (anyPlanet) ui << "----------------------------------------------------------------\n";

    if (!last_message.empty())
        ui << " > " << last_message << "\n----------------------------------------------------------------\n";

    ui << " COMMANDS: move <id> <x> <y> | attack <id> <target> | view <sector>\n";
    ui << "           next (advance turn) | status <id> | help | quit\n";
    ui << "================================================================\n";
    ui << " Input: ";

    std::cout << ui.str();
    std::cout.flush();
}

// ─── Main ─────────────────────────────────────────────────────────────────────

int main() {
    Map map(2, 3);

    Civilization player_civ("Human Empire", CivilizationType::PLAYER, Resource(300, 200, 150));
    Civilization enemy_civ ("Aggressors",   CivilizationType::AGGRESSIVE);

    const int f1 = map.addEntity(std::make_unique<Fighter>("F-1", Vec2{10, 5}, &player_civ));
    const int c1 = map.addEntity(std::make_unique<Cruiser>("C-1", Vec2{30, 8}, &player_civ));
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

    refreshFogOfWar(player, map);

    while (running) {
        drawUI(sr, player, player_civ, map, turn, message);
        message.clear();

        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        // --- Meta commands handled directly (instant) ---
        if (line == "quit") { running = false; continue; }

        if (line == "help") {
            std::ostringstream h;
            for (const auto& u : InputParser::getSupportedCommands()) h << u << "  |  ";
            message = h.str();
            continue;
        }

        if (line == "next") {
            // Advance the turn: tick all entities (auto-moves travelling ships), refresh FoW
            for (int r = 0; r < map.getChunkRows(); ++r)
                for (int c = 0; c < map.getChunkCols(); ++c) {
                    map.changeSelectedChunk(c, r);
                    // copy ids — update() may move ships out of this chunk
                    std::vector<int> ids(map.getSelectedChunk().getEntityIDs().begin(),
                                         map.getSelectedChunk().getEntityIDs().end());
                    for (int id : ids) {
                        Entity* e = map.getEntity(id);
                        if (e) { e->update(); map.updateEntityChunk(id); }
                    }
                }
            ++turn;
            refreshFogOfWar(player, map);
            message = "Turn advanced. Travelling ships moved closer to their destinations.";
            continue;
        }

        // --- status <id> ---
        if (line.rfind("status", 0) == 0) {
            std::istringstream ss(line);
            std::string cmd; int id;
            ss >> cmd >> id;
            const Entity* e = map.getEntity(id);
            const Ship* s = dynamic_cast<const Ship*>(e);
            message = s ? s->getDetailedInfo() : "No ship with that id.";
            continue;
        }

        // --- Game commands: parse and apply INSTANTLY ---
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
                    s->advanceTowardDestination();      // instant first step
                    map.updateEntityChunk(a.entity_id);
                    refreshFogOfWar(player, map);
                    if (s->reachedDestination())
                        message = s->getName() + " arrived at destination.";
                    else
                        message = s->getName() + " moving... use 'next' to continue travel.";
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
                    refreshFogOfWar(player, map);
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
            drawUI(sr, player, player_civ, map, turn, "All your ships destroyed. GAME OVER.");
            break;
        }
    }

    clearScreen();
    std::cout << "Game ended after " << turn << " turns. Thanks for playing!\n";
    return 0;
}