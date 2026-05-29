#include "../include/Render.hpp"
#include "../include/SubjectiveRender.hpp"
#include "../include/Map.hpp"
#include "../include/TurnManager.hpp"
#include "../include/InputParser.hpp"
#include "../include/Player.hpp"
#include "../include/Civilization.hpp"
#include "../include/Fighter.hpp"
#include "../include/Cruiser.hpp"
#include "../include/Planet.hpp"
#include "../include/CombatSystem.hpp"
#include <iostream>

int main() {
    // World: 2 rows x 3 cols = 6 chunks
    Map map(2, 3);

    // Civilizations
    Civilization player_civ("Human Empire", CivilizationType::PLAYER);
    Civilization enemy_civ("Aggressors",    CivilizationType::AGGRESSIVE);

    // Add ships
    const int f1_id = map.addEntity(std::make_unique<Fighter>("F-1", Vec2{10, 5},  &player_civ));
    const int c1_id = map.addEntity(std::make_unique<Cruiser>("C-1", Vec2{30, 5},  &player_civ));
    const int e1_id = map.addEntity(std::make_unique<Fighter>("E-1", Vec2{90, 12}, &enemy_civ));

    // Add a planet
    map.addEntity(std::make_unique<Planet>("Kepler", Vec2{50, 10},
                  PlanetType::MINERAL, Resource(500, 300, 200)));

    // Player setup
    Player player(1, "P1", "Human Player");
    player.addShipId(f1_id);
    player.addShipId(c1_id);

    // Systems
    Render renderer;
    SubjectiveRender subjective(renderer);
    CombatSystem combat;
    InputParser parser(map.getChunkRows() * map.getChunkCols());
    TurnManager turns(map, subjective, combat, player);

    // Turn 1
    turns.startTurn();
    turns.submitActions(parser.parseCommands({"move " + std::to_string(f1_id) + " 14 5"}));
    turns.executeTurn();
    turns.endTurn();

    return 0;
}