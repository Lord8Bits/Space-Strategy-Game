
#include "../include/Render.h"
#include "../include/Map.h"
#include "../include/Entity.hpp"
#include <chrono>
#include <iostream>

int main()
{
    // World: 2 rows × 3 cols = 6 chunks
    Map map(2, 3);

    // Only keep IDs for entities we'll reference later (movement, deletion)
    const int id1 = map.addEntity(std::make_unique<Entity>("Ship1", Vec2{10, 5},  'S'));
    const int id2 = map.addEntity(std::make_unique<Entity>("Ship2", Vec2{40, 19}, 'O'));
    map.addEntity(std::make_unique<Entity>("Ship3", Vec2{90, 12}, 'T'));
    map.addEntity(std::make_unique<Entity>("Ship4", Vec2{30, 25}, 'X'));

    Render renderer;

    // === Test Rendering ===
    std::cout << "=== Chunk (0,0): Ships S and O ===\n";
    renderer.drawWorld(map);

    std::cout << "=== Chunk (1,0): Ship T at screen (10,12) ===\n";
    map.changeSelectedChunk(1, 0);
    renderer.drawWorld(map);

    std::cout << "=== Chunk (0,1): Ship X at screen (30,5) ===\n";
    map.changeSelectedChunk(0, 1);
    renderer.drawWorld(map);

    // === Test Movement ===
    std::cout << "=== Moving Ship1 from chunk (0,0) to chunk (1,0) ===\n";
    Entity* ship1 = map.getEntity(id1);
    ship1->setPosition(Vec2{100, 5});
    map.updateEntityChunk(id1);

    map.changeSelectedChunk(0, 0);
    std::cout << "Chunk (0,0) - Ship1 should be gone:\n";
    renderer.drawWorld(map);

    map.changeSelectedChunk(1, 0);
    std::cout << "Chunk (1,0) - Ship1 should appear:\n";
    renderer.drawWorld(map);

    // === Test Deletion ===
    std::cout << "=== Deleting Ship2 ===\n";
    map.removeEntity(id2);

    map.changeSelectedChunk(0, 0);
    std::cout << "Chunk (0,0) - Ship2 should be gone:\n";
    renderer.drawWorld(map);

    // === Benchmark ===
    map.changeSelectedChunk(1, 0);
    constexpr int NUM_FRAMES = 1000;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_FRAMES; i++) renderer.drawWorld(map);
    auto end = std::chrono::high_resolution_clock::now();

    auto total_us  = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    auto per_frame = total_us / NUM_FRAMES;

    std::cout << "\n=== Benchmark ===\n";
    std::cout << "Total:     " << total_us  << " µs\n";
    std::cout << "Per frame: " << per_frame << " µs\n";
    std::cout << "FPS:       " << (1000000.0 / per_frame) << "\n";

    return 0;
}