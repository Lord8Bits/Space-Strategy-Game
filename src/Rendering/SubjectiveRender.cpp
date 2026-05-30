#include "../../include/SubjectiveRender.hpp"
#include "../../include/Civilization.hpp"
#include "../../include/Ship.hpp"
#include "../../include/Planet.hpp"
#include <iostream>
#include <sstream>

void SubjectiveRender::draw(const Player& player, const Map& map) {
    const Chunk&      chunk       = map.getSelectedChunk();
    const int         world_width = map.getWorldWidth();
    const ViewPort    vp(chunk.getXStart(), chunk.getYStart());
    const Perception& perc        = player.getPerception();

    // Step 1: Fill buffer with fog (Hidden = space, no color)
    _viewport.fill({' ', GameUI::Color::BLACK});

    // Step 2: Place currently visible entities into the buffer
    for (const int entity_id : chunk.getEntityIDs()) {
        const Entity* entity = map.getEntity(entity_id);
        if (!entity) continue;

        const Vec2 world_pos = entity->getPosition();
        if (!vp.isInViewport(world_pos.x, world_pos.y)) continue;

        if (classifyCell(perc, world_pos.x, world_pos.y, world_width) != CellVisibility::Visible)
            continue;

        _viewport[vp.toIndex(world_pos.x, world_pos.y)] = Render::makeCell(*entity);
    }

    // Step 3: Apply fog/memory overlay to empty cells
    for (int y = 0; y < VIEWPORT_HEIGHT; ++y) {
        for (int x = 0; x < VIEWPORT_WIDTH; ++x) {
            const int world_x = chunk.getXStart() + x;
            const int world_y = chunk.getYStart() + y;
            const int idx     = vp.toIndex(world_x, world_y);

            if (_viewport[idx].symbol != ' ') continue;  // entity already placed here

            switch (classifyCell(perc, world_x, world_y, world_width)) {
                case CellVisibility::Visible: _viewport[idx] = {'.', GameUI::Color::WHITE}; break;
                case CellVisibility::Memory:  _viewport[idx] = {'~', GameUI::Color::BLACK}; break;
                case CellVisibility::Hidden:  _viewport[idx] = {' ', GameUI::Color::BLACK}; break;
            }
        }
    }

    // Step 4: Build ANSI frame buffer and flush
    GameUI::Color last_color = GameUI::Color::BLACK;
    _frame_buffer  = "\033[H";
    _frame_buffer += GameUI::toAnsi(last_color);

    for (int y = 0; y < VIEWPORT_HEIGHT; ++y) {
        for (int x = 0; x < VIEWPORT_WIDTH; ++x) {
            const int idx = vp.toIndex(chunk.getXStart() + x, chunk.getYStart() + y);
            if (last_color != _viewport[idx].color) {
                _frame_buffer += GameUI::toAnsi(_viewport[idx].color);
                last_color = _viewport[idx].color;
            }
            _frame_buffer += _viewport[idx].symbol;
        }
        _frame_buffer += '\n';
    }

    _frame_buffer += GameUI::toAnsi(GameUI::Color::RESET);
    std::cout << _frame_buffer;
    std::cout.flush();
}

void SubjectiveRender::drawUI(const Player& player, const Civilization& civ,
                               Map& map, int turn, const std::string& last_message) {
    Render::clearScreen();
    draw(player, map);

    const int       selected = map.getSelectedChunkIndex() + 1;
    const int       total    = map.getChunkRows() * map.getChunkCols();
    const Resource& res      = civ.getResources();

    std::ostringstream ui;
    ui << "================================================================\n";
    ui << " Turn " << turn
       << "  |  Civ: " << civ.getName()
       << "  |  Viewing Sector " << selected << "/" << total
       << "  |  Explored " << static_cast<int>(player.getPerception().explorationRatio() * 100) << "%\n";
    ui << " Resources -> Gold: " << res.getGold()
       << "   Titanium: " << res.getTitanium()
       << "   Cadmium: "  << res.getCadmium() << "\n";
    ui << "----------------------------------------------------------------\n";
    ui << " YOUR FLEET:\n";
    for (const int id : player.getShipIds()) {
        const Entity* e = map.getEntity(id);
        if (!e) continue;
        const Ship* s = dynamic_cast<const Ship*>(e);
        if (!s) continue;
        ui << "   " << s->getDetailedInfo()
           << "  (sector " << map.sectorOf(s->getPosition()) << ")\n";
    }
    ui << "----------------------------------------------------------------\n";

    // List all discovered planets
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
                   << "  (sector " << map.sectorOf(pp) << ")\n";
            }
        }
    }
    // Restore the sector the player was viewing before the iteration above
    const int sel = selected - 1;
    map.changeSelectedChunk(sel % map.getChunkCols(), sel / map.getChunkCols());

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
