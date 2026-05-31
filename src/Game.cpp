#include "../include/Game.hpp"
#include "../include/Fighter.hpp"
#include "../include/Cruiser.hpp"
#include "../include/Transport.hpp"
#include "../include/Planet.hpp"
#include "../src/Utils/Constants.hpp"
#include <iostream>
#include <sstream>

// ─── Vision helper ────────────────────────────────────────────────────────────

void Game::applyVision() {
    _player.refreshFogOfWar(_map, _player_civ);
    if (_devFogEnabled) _player.getPerception().revealAll();
}

// ─── Constructor ──────────────────────────────────────────────────────────────

Game::Game()
    : _player_civ("Human Empire", CivilizationType::PLAYER,   Resource(300, 200, 150))
    , _enemy_civ ("Aggressors",   CivilizationType::AGGRESSIVE)
    , _map(2, 3)
    , _player(1, "P1", "Human Player")
    , _combat()
    , _renderer()
    , _sr(_renderer)
    , _parser(_map.getChunkRows() * _map.getChunkCols())
{
    initWorld();
}

// ─── World setup ──────────────────────────────────────────────────────────────

void Game::initWorld() {
    // ── Player fleet (near home planet at 40,10) ──────────────────────────────
    const int f1 = _map.addEntity(std::make_unique<Fighter>  ("F-1", Vec2{35, 8},  &_player_civ));
    const int c1 = _map.addEntity(std::make_unique<Cruiser>  ("C-1", Vec2{45, 10}, &_player_civ));
    const int t1 = _map.addEntity(std::make_unique<Transport>("T-1", Vec2{40, 14}, &_player_civ));
    _ship_count = 3;

    // ── Player home planet ────────────────────────────────────────────────────
    _home_id = _map.addEntity(std::make_unique<Planet>(
        "Terra", Vec2{40, 10}, PlanetType::TERRAIN, Resource(400, 300, 200), &_player_civ));

    // ── Other planets ─────────────────────────────────────────────────────────
    _map.addEntity(std::make_unique<Planet>("Kepler",  Vec2{130, 8},  PlanetType::MINERAL, Resource(600, 400, 100)));
    _map.addEntity(std::make_unique<Planet>("Solaris", Vec2{160, 15}, PlanetType::ENERGY,  Resource(200, 100, 500)));

    // ── Enemy fleet (sector 2) ────────────────────────────────────────────────
    const int e1 = _map.addEntity(std::make_unique<Fighter>("E-1", Vec2{90,  5},  &_enemy_civ));
    const int e2 = _map.addEntity(std::make_unique<Cruiser>("E-2", Vec2{100, 10}, &_enemy_civ));

    // ── Player setup ──────────────────────────────────────────────────────────
    _player.addShipId(f1); _player.addShipId(c1); _player.addShipId(t1);

    _player_civ.addEntity(f1); _player_civ.addEntity(c1); _player_civ.addEntity(t1);
    _player_civ.addEntity(_home_id);
    _enemy_civ.addEntity(e1); _enemy_civ.addEntity(e2);

    // Start Plasma Cannons at level 1 for the player
    _player_civ.upgradeWeaponTech();

    // ── Pre-discover the home sector entirely (radius 51 covers full 80×20 chunk) ──
    _player.getPerception().updateVisibility(
        40, 10, 51, _map.getWorldWidth(), _map.getWorldHeight());
    applyVision();
}

// ─── Turn advance ─────────────────────────────────────────────────────────────

void Game::advanceTurn() {
    const int saved = _map.getSelectedChunkIndex();

    // Step 1: Reset all entities (movement points, states) — no movement yet.
    // AI must act with full MP before any auto-travel consumes the budget.
    for (int r = 0; r < _map.getChunkRows(); ++r) {
        for (int c = 0; c < _map.getChunkCols(); ++c) {
            _map.changeSelectedChunk(c, r);
            for (int id : _map.getSelectedChunk().getEntityIDs()) {
                Entity* e = _map.getEntity(id);
                if (e) e->update();
            }
        }
    }

    // Step 2: Enemy AI acts — each ship has full MP, so it can move AND attack.
    _enemy_civ.takeTurn(_map, _combat);

    // Step 3: Auto-advance player ships that have a pending destination.
    // Enemy ships moved themselves in step 2; only player ships need this.
    for (int id : _player.getShipIds()) {
        Entity* e = _map.getEntity(id);
        Ship*   s = e ? e->asShip() : nullptr;
        if (s) { s->advancePendingMovement(); _map.updateEntityChunk(id); }
    }

    // Step 3: clean up ships destroyed this turn (both sides)
    {
        // Player ships killed by AI attacks
        std::vector<int> player_killed;
        for (int id : _player.getShipIds()) {
            Entity* e = _map.getEntity(id);
            if (e && !e->isAlive()) player_killed.push_back(id);
        }
        for (int id : player_killed) {
            _player.removeShipId(id);
            _player_civ.removeEntity(id);
            _map.removeEntity(id);
        }

        // Enemy ships killed by player counter-attacks during the AI turn
        std::vector<int> enemy_killed;
        for (int id : _enemy_civ.getEntityIDs()) {
            Entity* e = _map.getEntity(id);
            if (e && e->asShip() && !e->isAlive()) enemy_killed.push_back(id);
        }
        for (int id : enemy_killed) {
            _enemy_civ.removeEntity(id);
            _map.removeEntity(id);
        }
    }

    _map.changeSelectedChunk(saved % _map.getChunkCols(), saved / _map.getChunkCols());
    ++_turn;
    applyVision();
}

// ─── Action execution ─────────────────────────────────────────────────────────

std::string Game::executeAction(const Action& a) {
    switch (a.action_type) {

        // ── VIEW ──────────────────────────────────────────────────────────────
        case Action::Type::VIEW_SECTOR: {
            const int s = a.sector_id - 1;
            _map.changeSelectedChunk(s % _map.getChunkCols(), s / _map.getChunkCols());
            return "Now viewing sector " + std::to_string(a.sector_id) + ".";
        }

        // ── MOVE ──────────────────────────────────────────────────────────────
        case Action::Type::MOVE: {
            Entity* e = _map.getEntity(a.entity_id);
            Ship*   s = e ? e->asShip() : nullptr;
            if (!s || !s->isAlive())           return "No controllable ship with that id.";
            if (s->getMovementPoints() <= 0)   return s->getName() + " has no movement points. Press 'next'.";

            const Chunk& viewed = _map.getSelectedChunk();
            const Vec2 world_pos{
                viewed.getXStart() + (a.target_position.x - 1),
                viewed.getYStart() +  a.target_position.y
            };
            s->setDestination(world_pos);
            s->advanceTowardDestination();
            _map.updateEntityChunk(a.entity_id);
            applyVision();

            const char y_letter = static_cast<char>('A' + a.target_position.y);
            const std::string coord = std::string(1, y_letter) + std::to_string(a.target_position.x);
            return s->reachedDestination()
                ? s->getName() + " arrived at " + coord + " (world " + std::to_string(world_pos.x) + "," + std::to_string(world_pos.y) + ")."
                : s->getName() + " moving toward " + coord + ". Use 'next' to continue.";
        }

        // ── ATTACK ────────────────────────────────────────────────────────────
        case Action::Type::ATTACK: {
            Entity* atkE = _map.getEntity(a.entity_id);
            Entity* defE = _map.getEntity(a.target_entity_id);
            Ship*   atk  = atkE ? atkE->asShip() : nullptr;
            if (!atk || !atk->isAlive() || !defE)  return "Invalid attacker or target.";
            if (!atk->canAttack())                  return atk->getName() + " has no movement points to attack!";

            _combat.setCurrentTurn(_turn);
            const CombatResult res = atk->attack(*defE, _combat);
            std::ostringstream log;
            for (const auto& l : res.logs) log << l.message << " | ";

            if (res.isDestroyed) {
                // If a Transport was destroyed at a planet, the attacker's civ colonizes it
                const Ship* defShip = defE->asShip();
                if (defShip && defShip->canMine()) {
                    Entity* pe = _map.findPlanetAt(defE->getPosition());
                    Planet* p  = pe ? pe->asPlanet() : nullptr;
                    if (p && p->getCivOwner() != atk->getCivOwner()) {
                        p->colonize(atk->getCivOwner());
                        if (atk->getCivOwner()) atk->getCivOwner()->addEntity(pe->getId());
                    }
                }
                _player.removeShipId(a.target_entity_id);
                _enemy_civ.removeEntity(a.target_entity_id);
                _map.removeEntity(a.target_entity_id);
            }
            if (res.attackerDestroyed) {
                _player.removeShipId(a.entity_id);
                _player_civ.removeEntity(a.entity_id);
                _map.removeEntity(a.entity_id);
            }
            applyVision();
            return log.str();
        }

        // ── BUILD (always at home planet) ─────────────────────────────────────
        case Action::Type::BUILD: {
            Entity* homeE = _map.getEntity(_home_id);
            Planet* home  = homeE ? homeE->asPlanet() : nullptr;
            if (!home || home->getCivOwner() != &_player_civ)
                return "Home planet not found or lost!";

            const Vec2 spawn = home->getPosition();
            Resource cost(0, 0, 0);
            std::string type_name;
            std::unique_ptr<Ship> new_ship;
            ++_ship_count;

            if (a.value == 0) {
                cost      = Resource(GameConstants::FIGHTER_BUILD_GOLD,
                                     GameConstants::FIGHTER_BUILD_TITANIUM,
                                     GameConstants::FIGHTER_BUILD_CADMIUM);
                type_name = "Fighter";
                new_ship  = std::make_unique<Fighter>("F-" + std::to_string(_ship_count), spawn, &_player_civ);
            } else if (a.value == 1) {
                cost      = Resource(GameConstants::CRUISER_BUILD_GOLD,
                                     GameConstants::CRUISER_BUILD_TITANIUM,
                                     GameConstants::CRUISER_BUILD_CADMIUM);
                type_name = "Cruiser";
                new_ship  = std::make_unique<Cruiser>("C-" + std::to_string(_ship_count), spawn, &_player_civ);
            } else {
                cost      = Resource(GameConstants::TRANSPORT_BUILD_GOLD,
                                     GameConstants::TRANSPORT_BUILD_TITANIUM,
                                     GameConstants::TRANSPORT_BUILD_CADMIUM);
                type_name = "Transport";
                new_ship  = std::make_unique<Transport>("T-" + std::to_string(_ship_count), spawn, &_player_civ);
            }

            if (!_player_civ.getResources().canAfford(cost)) {
                --_ship_count;
                return "Insufficient resources. Need Gold:" + std::to_string(cost.getGold())
                     + " Titanium:" + std::to_string(cost.getTitanium())
                     + " Cadmium:" + std::to_string(cost.getCadmium()) + ".";
            }
            _player_civ.getResources() -= cost;
            const int new_id = _map.addEntity(std::move(new_ship));
            _player.addShipId(new_id);
            _player_civ.addEntity(new_id);
            applyVision();
            return type_name + " [" + std::to_string(new_id) + "] built at Terra.";
        }

        // ── COLONIZE (Transport at planet position) ───────────────────────────
        case Action::Type::COLONIZE: {
            Entity* e = _map.getEntity(a.entity_id);
            Ship*   s = e ? e->asShip() : nullptr;
            if (!s || !s->isAlive() || !s->canMine())
                return "Only a Transport can colonize — bring one to the planet.";

            Entity* pe = _map.findPlanetAt(s->getPosition());
            Planet* p  = pe ? pe->asPlanet() : nullptr;
            if (!p)
                return "No planet at " + _map.toViewportCoord(s->getPosition()) + ".";
            if (p->getCivOwner() == &_player_civ)
                return p->getName() + " is already yours.";
            if (p->getCivOwner() == &_enemy_civ)
                return "Cannot colonize an enemy planet — destroy their ships there first.";

            p->colonize(&_player_civ);
            _player_civ.addEntity(pe->getId());
            applyVision();
            return p->getName() + " colonized for " + _player_civ.getName() + "!";
        }

        // ── MINE (Transport at planet position) ────────────────────────────────
        case Action::Type::MINE: {
            Entity* e = _map.getEntity(a.entity_id);
            Ship*   s = e ? e->asShip() : nullptr;
            if (!s || !s->isAlive())   return "No ship with that id.";
            if (!s->canMine())         return s->getName() + " cannot mine — only Transports can.";

            Entity* pe = _map.findPlanetAt(s->getPosition());
            Planet* p  = pe ? pe->asPlanet() : nullptr;
            if (!p)    return "No planet at " + _map.toViewportCoord(s->getPosition()) + " to mine.";
            if (p->getCivOwner() == &_enemy_civ)
                       return "Cannot mine an enemy planet.";

            const Resource extracted = p->extract(
                Resource(GameConstants::MINE_AMOUNT, GameConstants::MINE_AMOUNT, GameConstants::MINE_AMOUNT));
            _player_civ.getResources() += extracted;
            return "Mined from " + p->getName() + ": +" + std::to_string(extracted.getGold())
                 + "G +" + std::to_string(extracted.getTitanium())
                 + "T +" + std::to_string(extracted.getCadmium()) + "C";
        }

        // ── RESEARCH ──────────────────────────────────────────────────────────
        case Action::Type::RESEARCH: {
            if (!_player_civ.canUpgradeWeaponTech())
                return "Plasma Cannons already at max level!";
            const Resource cost = _player_civ.getWeaponTechUpgradeCost();
            if (!_player_civ.getResources().canAfford(cost))
                return "Insufficient resources for research. Need Gold:" + std::to_string(cost.getGold())
                     + " Titanium:" + std::to_string(cost.getTitanium())
                     + " Cadmium:" + std::to_string(cost.getCadmium()) + ".";
            _player_civ.getResources() -= cost;
            _player_civ.upgradeWeaponTech();
            return "Plasma Cannons upgraded to level "
                 + std::to_string(_player_civ.getWeaponTech().getLevel())
                 + "! Fighter ATK bonus: +" + std::to_string(_player_civ.getAttackBonus());
        }

        // ── CANCEL ────────────────────────────────────────────────────────────
        case Action::Type::CANCEL: {
            Entity* e = _map.getEntity(a.entity_id);
            Ship*   s = e ? e->asShip() : nullptr;
            if (!s || !s->isAlive()) return "No controllable ship with that id.";
            if (!s->hasDestination())
                return s->getName() + " has no pending action to cancel.";
            s->cancelAction();
            return s->getName() + " stopped — now Idle at " + _map.toViewportCoord(s->getPosition()) + ".";
        }

        // ── DEVFOG ────────────────────────────────────────────────────────────
        case Action::Type::DEVFOG:
            _devFogEnabled = !_devFogEnabled;
            applyVision();
            return _devFogEnabled ? "[DEV] Full visibility ON  — type devfog again to disable."
                                  : "[DEV] Full visibility OFF — fog of war restored.";

        default:
            return "That command isn't wired in yet.";
    }
}

// ─── Main loop ────────────────────────────────────────────────────────────────

void Game::run() {
    std::string message = "Welcome, Commander. Home world Terra is at "
                        + _map.toViewportCoord({40, 10}) + " (sector 1).";

    while (true) {
        _sr.drawUI(_player, _player_civ, _map, _turn, message);
        message.clear();

        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        // ── Meta commands ─────────────────────────────────────────────────────
        if (line == "quit") break;

        if (line == "help") {
            std::ostringstream h;
            for (const auto& u : InputParser::getSupportedCommands())
                h << "  " << u << "\n";
            message = h.str();
            continue;
        }

        if (line == "next") {
            advanceTurn();
            message = "Turn " + std::to_string(_turn) + " started. Ships replenished movement points.";
            continue;
        }

        if (line.rfind("status", 0) == 0) {
            std::istringstream ss(line);
            std::string cmd; int id;
            ss >> cmd >> id;
            const Entity* e = _map.getEntity(id);
            if (!e) { message = "No entity with id " + std::to_string(id) + "."; continue; }
            message = e->getDetailedInfo()
                    + "\n  Viewport: " + _map.toViewportCoord(e->getPosition())
                    + "  Sector "     + std::to_string(_map.sectorOf(e->getPosition()));
            continue;
        }

        // ── Game commands ─────────────────────────────────────────────────────
        try {
            message = executeAction(_parser.parseCommand(line));
        } catch (const std::exception& ex) {
            message = InputParser::getErrorMessage(ex);
        }

        if (_player.isDefeated()) {
            _sr.drawUI(_player, _player_civ, _map, _turn, "All ships lost — GAME OVER.");
            break;
        }
    }

    Render::clearScreen();
    std::cout << "Game ended after " << _turn << " turns. Thanks for playing!\n";
}
