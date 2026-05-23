#include "../include/Render.h"
#include "../include/Map.h"
#include "../include/Entity.hpp"
#include <chrono>
#include <iostream>

int main()
{
    // World layout: 2 rows × 3 cols = 6 chunks total
    // Chunk 0: world (0,0)→(80,20)   Chunk 1: world (80,0)→(160,20)  Chunk 2: world (160,0)→(240,20)
    // Chunk 3: world (0,20)→(80,40)  Chunk 4: world (80,20)→(160,40) Chunk 5: world (160,20)→(240,40)
    Map map(2, 3);

    // Entities in chunk 0 (world x: 0-80, y: 0-20)
    auto ship1 = std::make_unique<Entity>(
        EntityType::SHIP, "Ship1",
        Vec2{10, 5},
        Cell{'S', GameUI::Color::GREEN}
    );
    auto ship2 = std::make_unique<Entity>(
        EntityType::SHIP, "Ship2",
        Vec2{40, 19},
        Cell{'O', GameUI::Color::RED}
    );

    // Entity in chunk 1 (world x: 80-160, y: 0-20)
    auto ship3 = std::make_unique<Entity>(
        EntityType::SHIP, "Ship3",
        Vec2{90, 12},
        Cell{'T', GameUI::Color::YELLOW}
    );

    // Entity in chunk 3 (world x: 0-80, y: 20-40)
    auto ship4 = std::make_unique<Entity>(
        EntityType::SHIP, "Ship4",
        Vec2{30, 25},
        Cell{'X', GameUI::Color::CYAN}
    );

    map.addEntity(std::move(ship1));
    map.addEntity(std::move(ship2));
    map.addEntity(std::move(ship3));
    map.addEntity(std::move(ship4));

    Render renderer;

    // Test chunk 0
    std::cout << "=== Chunk 0 (ships at (10,5) and (40,19)) ===\n";
    renderer.drawWorld(map);

    // Test chunk 1
    std::cout << "=== Chunk 1 (ship at (90,12) → screen (10,12)) ===\n";
    map.changeSelectedChunk(1, 0);
    renderer.drawWorld(map);

    // Test chunk 3
    std::cout << "=== Chunk 3 (ship at (30,25) → screen (30,5)) ===\n";
    map.changeSelectedChunk(0, 1);
    renderer.drawWorld(map);

    // // Benchmark
    // std::cout << "=== Benchmark (1000 frames on chunk 0) ===\n";
    // map.changeSelectedChunk({}, {});
    // const int NUM_FRAMES = 1000;
    // auto start = std::chrono::high_resolution_clock::now();
    // for (int i = 0; i < NUM_FRAMES; i++) {
    //     renderer.drawWorld(map);
    // }
    // auto end = std::chrono::high_resolution_clock::now();
    // auto total     = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    // auto per_frame = total / NUM_FRAMES;
    //
    // std::cout << "Total:     " << total     << " µs\n";
    // std::cout << "Per frame: " << per_frame << " µs\n";
    // std::cout << "FPS:       " << (1000000.0 / per_frame) << "\n";

    return 0;
}
