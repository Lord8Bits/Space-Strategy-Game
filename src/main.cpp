// main.cpp - This should WORK when you run it

#include "../include/Render.h"
#include "../include/Map.h"
#include "../include/Chunk.h"
#include "../include/Entity.hpp"
#include <chrono> // For benchmarking
#include <iostream>
int main()
{
    // Create a chunk
    Chunk chunk1(0, 1600);  // covers first 1600 cells of world buffer
    Chunk chunk2(1600, 3200);  // covers first 1600 cells of world buffer

    auto ship1 = std::make_unique<Entity>(
        EntityType::SHIP,
        "Ship1",
        Vec2{10, 5},  // Position in world
        Cell{'S', GameUI::Color::GREEN}
    );

    auto ship2 = std::make_unique<Entity>(
        EntityType::SHIP,
        "Ship2",
        Vec2{40, 19},  // Position in world
        Cell{'O', GameUI::Color::RED}
    );

    auto ship3 = std::make_unique<Entity>(
        EntityType::SHIP,
        "Ship3",
        Vec2{76, 12},  // Position in world
        Cell{'T', GameUI::Color::YELLOW}
    );

    chunk1.addEntity(std::move(ship1));
    chunk1.addEntity(std::move(ship2));
    chunk2.addEntity(std::move(ship3));

    Map map;
    map.addChunk(std::move(chunk1));
    map.addChunk(std::move(chunk2));

    Render renderer;
    map.changeSelectedChunk(1);
    const int NUM_FRAMES = 1000;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_FRAMES; i++) {
        renderer.drawWorld(map);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    auto per_frame = total.count() / NUM_FRAMES;

    std::cout << "Total time: " << total.count() << " microseconds\n";
    std::cout << "Per frame: " << per_frame << " microseconds\n";
    std::cout << "FPS: " << (1000000.0 / per_frame) << "\n";

    return 0;
}
