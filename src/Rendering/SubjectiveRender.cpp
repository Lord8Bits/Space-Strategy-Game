#include "../../include/SubjectiveRender.hpp"
#include "../../include/Civilization.hpp"
#include "../../include/Ship.hpp"
#include "../../include/Planet.hpp"
#include <iostream>
#include <sstream>

// ─── draw ─────────────────────────────────────────────────────────────────────

void SubjectiveRender::draw(const Player& player, const Map& map) {
    const Chunk&      chunk       = map.getSelectedChunk();
    const int         world_width = map.getWorldWidth();
    const ViewPort    vp(chunk.getXStart(), chunk.getYStart());
    const Perception& perc        = player.getPerception();

    // Step 1: Fill with total fog
    _viewport.fill({" ", GameUI::Color::BLACK});

    // Step 2: Place visible entities
    for (const int id : chunk.getEntityIDs()) {
        const Entity* entity = map.getEntity(id);
        if (!entity) continue;
        const Vec2 wp = entity->getPosition();
        if (!vp.isInViewport(wp.x, wp.y)) continue;
        if (classifyCell(perc, wp.x, wp.y, world_width) != CellVisibility::Visible) continue;
        _viewport[vp.toIndex(wp.x, wp.y)] = Render::makeCell(*entity);
    }

    // Step 3: Fog / memory overlay on empty cells
    for (int y = 0; y < VIEWPORT_HEIGHT; ++y) {
        for (int x = 0; x < VIEWPORT_WIDTH; ++x) {
            const int wx  = chunk.getXStart() + x;
            const int wy  = chunk.getYStart() + y;
            const int idx = vp.toIndex(wx, wy);
            if (_viewport[idx].symbol != " ") continue;
            switch (classifyCell(perc, wx, wy, world_width)) {
                case CellVisibility::Visible: _viewport[idx] = {".", GameUI::Color::WHITE}; break;
                case CellVisibility::Memory:  _viewport[idx] = {"~", GameUI::Color::BLACK}; break;
                case CellVisibility::Hidden:  _viewport[idx] = {" ", GameUI::Color::BLACK}; break;
            }
        }
    }

    // Step 4: Build ANSI frame with chess-style coordinate border
    _frame_buffer  = "\033[H";
    GameUI::Color last_color = GameUI::Color::WHITE;
    _frame_buffer += GameUI::toAnsi(last_color);

    // Top X-axis header — viewport columns 1-80
    {
        std::string x_header(VIEWPORT_WIDTH, ' ');
        x_header[0] = '1';
        for (int col = 10; col <= 70; col += 10) {
            const std::string lbl = std::to_string(col);
            const int pos = col - 1;
            for (int i = 0; i < static_cast<int>(lbl.size()); ++i) x_header[pos + i] = lbl[i];
        }
        x_header[78] = '8'; x_header[79] = '0';
        _frame_buffer += "   |" + x_header + '\n';
        _frame_buffer += "---+" + std::string(VIEWPORT_WIDTH, '-') + '\n';
    }

    // Rows with left Y-axis letter label (A=row 0 … T=row 19)
    for (int y = 0; y < VIEWPORT_HEIGHT; ++y) {
        if (last_color != GameUI::Color::WHITE) {
            _frame_buffer += GameUI::toAnsi(GameUI::Color::WHITE);
            last_color = GameUI::Color::WHITE;
        }
        const char y_letter = static_cast<char>('A' + y);
        _frame_buffer += "  " + std::string(1, y_letter) + "|";

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

// ─── drawUI ───────────────────────────────────────────────────────────────────

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
       << "  |  " << civ.getName()
       << "  |  Sector " << selected << "/" << total
       << "  |  Explored " << static_cast<int>(player.getPerception().explorationRatio() * 100) << "%\n";
    ui << " Resources: Gold=" << res.getGold()
       << "  Titanium=" << res.getTitanium()
       << "  Cadmium=" << res.getCadmium()
       << "  |  Plasma Cannons Lv" << civ.getWeaponTech().getLevel()
       << " (+" << civ.getAttackBonus() << " Fighter ATK)\n";
    ui << "----------------------------------------------------------------\n";

    // ── Fleet ────────────────────────────────────────────────────────────────
    ui << " YOUR FLEET:\n";
    for (const int id : player.getShipIds()) {
        const Entity* e = map.getEntity(id);
        if (!e) continue;
        // getDetailedInfo() is virtual — no cast needed
        ui << "   " << e->getDetailedInfo()
           << "  @" << map.toViewportCoord(e->getPosition())
           << "  (sector " << map.sectorOf(e->getPosition()) << ")\n";
    }
    ui << "----------------------------------------------------------------\n";

    // ── Single pass: collect visible enemies and known planets ────────────────
    std::ostringstream enemy_buf, planet_buf;
    bool anyEnemy = false, anyPlanet = false;

    for (int r = 0; r < map.getChunkRows(); ++r) {
        for (int c = 0; c < map.getChunkCols(); ++c) {
            map.changeSelectedChunk(c, r);
            for (const int id : map.getSelectedChunk().getEntityIDs()) {
                const Entity* e = map.getEntity(id);
                if (!e) continue;
                const Vec2 pos = e->getPosition();

                // Visible enemies
                if (e->isAlive() && e->getCivOwner() && e->getCivOwner() != &civ) {
                    if (player.getPerception().isVisible(pos.x, pos.y, map.getWorldWidth())) {
                        anyEnemy = true;
                        enemy_buf << "   [" << id << "] " << e->getName()
                                  << "  " << e->getCivOwner()->getName();
                        // Use virtual getters — no cast
                        if (e->getMaxHealth() > 0)
                            enemy_buf << "  HP:" << e->getHealth() << "/" << e->getMaxHealth();
                        enemy_buf << "  @" << map.toViewportCoord(pos)
                                  << "  (sector " << map.sectorOf(pos) << ")\n";
                    }
                }

                // Known planets — use asPlanet() self-cast
                const Planet* p = e->asPlanet();
                if (p && player.getPerception().isDiscovered(pos.x, pos.y, map.getWorldWidth())) {
                    anyPlanet = true;
                    planet_buf << "   [" << id << "] " << p->getName()
                               << "  @" << map.toViewportCoord(pos)
                               << "  resources:" << p->getResourceCount()
                               << "  (sector " << map.sectorOf(pos) << ")";
                    if (p->isColonized())
                        planet_buf << "  [" << p->getCivOwner()->getName() << "]";
                    planet_buf << "\n";
                }
            }
        }
    }

    // Restore the sector the player was viewing
    const int sel = selected - 1;
    map.changeSelectedChunk(sel % map.getChunkCols(), sel / map.getChunkCols());

    if (anyEnemy) {
        ui << " VISIBLE ENEMIES:\n" << enemy_buf.str();
        ui << "----------------------------------------------------------------\n";
    }
    if (anyPlanet) {
        ui << " KNOWN PLANETS:\n" << planet_buf.str();
        ui << "----------------------------------------------------------------\n";
    }

    if (!last_message.empty())
        ui << " > " << last_message << "\n----------------------------------------------------------------\n";

    ui << " COMMANDS: move <id> <coord>  (e.g. A20 or T5 — relative to viewed sector)\n";
    ui << "           attack <id> <target_id>  |  mine <transport_id>  |  research\n";
    ui << "           build <fighter|cruiser|transport>  |  view <sector>  |  next\n";
    ui << "           status <id>  |  help  |  quit\n";
    ui << " LEGEND: ▲=Fighter  ◆=Cruiser  ■=Transport  ●=Planet (green/yellow/cyan)\n";
    ui << "================================================================\n";
    ui << " Input: ";

    std::cout << ui.str();
    std::cout.flush();
}
