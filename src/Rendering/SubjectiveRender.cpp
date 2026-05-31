#include "Rendering/SubjectiveRender.hpp"
#include "AI/Civilization.hpp"
#include "Entities/Ship.hpp"
#include "Entities/Planet.hpp"
#include <iostream>
#include <sstream>

// Returns RED for any entity whose owner is currently an enemy of playerCiv,
// otherwise falls back to the entity's own display color.
static GameUI::Color resolveEntityColor(const Entity& entity, const Civilization& playerCiv) {
    const Civilization* owner = entity.getCivOwner();
    if (owner && owner != &playerCiv
        && playerCiv.getRelationWith(*owner) == Relation::ENEMY)
        return GameUI::Color::RED;
    return entity.getDisplayColor();
}

// ── Compact ANSI palette for the UI panel ─────────────────────────────────────
namespace {
    constexpr std::string_view R    = "\x1b[0m";      // reset
    constexpr std::string_view DIM  = "\x1b[2m";      // dimmed
    constexpr std::string_view BOLD = "\x1b[1m";
    constexpr std::string_view CYN  = "\x1b[36m";     // cyan
    constexpr std::string_view YEL  = "\x1b[33m";     // yellow
    constexpr std::string_view GRN  = "\x1b[32m";     // green
    constexpr std::string_view RED  = "\x1b[31m";     // red
    constexpr std::string_view WHT  = "\x1b[37m";     // white
    constexpr std::string_view BCYN = "\x1b[1;36m";   // bold cyan
    constexpr std::string_view BYEL = "\x1b[1;33m";   // bold yellow
    constexpr std::string_view BGRN = "\x1b[1;32m";   // bold green
    constexpr std::string_view BRED = "\x1b[1;31m";   // bold red
    constexpr std::string_view DGRY = "\x1b[90m";     // dark gray

    std::string_view relationTextColor(const Civilization& viewer, const Civilization* owner) {
        if (!owner) return WHT;
        if (owner == &viewer) return CYN;

        switch (viewer.getRelationWith(*owner)) {
            case Relation::ALLY:    return GRN;
            case Relation::NEUTRAL: return DGRY;
            case Relation::ENEMY:   return RED;
            default:                return WHT;
        }
    }

    std::string_view relationBrightTextColor(const Civilization& viewer, const Civilization* owner) {
        if (!owner) return WHT;
        if (owner == &viewer) return BCYN;

        switch (viewer.getRelationWith(*owner)) {
            case Relation::ALLY:    return BGRN;
            case Relation::NEUTRAL: return WHT;
            case Relation::ENEMY:   return BRED;
            default:                return WHT;
        }
    }
}

// ─── draw ─────────────────────────────────────────────────────────────────────

void SubjectiveRender::draw(const Player& player, const Map& map, const Civilization* playerCiv) {
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
        GameUI::Cell cell = Render::makeCell(*entity);
        if (playerCiv) cell.color = resolveEntityColor(*entity, *playerCiv);
        _viewport[vp.toIndex(wp.x, wp.y)] = cell;
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
    draw(player, map, &civ);

    const int       selected = map.getSelectedChunkIndex() + 1;
    const int       total    = map.getChunkRows() * map.getChunkCols();
    const Resource& res      = civ.getResources();

    std::ostringstream ui;

    // ── Header bar ────────────────────────────────────────────────────────────
    ui << DGRY << "================================================================" << R << "\n";
    ui << " " << BYEL << "Turn " << turn << R
       << "  |  " << BCYN << civ.getName() << R
       << "  |  " << WHT << "Sector " << selected << "/" << total << R
       << "  |  " << GRN << "Explored " << static_cast<int>(player.getPerception().explorationRatio() * 100) << "%" << R << "\n";

    // ── Resources ─────────────────────────────────────────────────────────────
    ui << " " << WHT << "Resources:" << R
       << "  " << YEL << "Gold "     << BYEL << res.getGold()      << R
       << "   " << CYN << "Titanium " << BCYN << res.getTitanium()  << R
       << "   " << GRN << "Cadmium "  << BGRN << res.getCadmium()   << R
       << "   " << DGRY << "|" << R
       << "  " << CYN << "Plasma Cannons Lv" << BCYN << civ.getWeaponTech().getLevel() << R
       << CYN << " (+" << civ.getAttackBonus() << " Fighter ATK)" << R << "\n";
    ui << DGRY << "----------------------------------------------------------------" << R << "\n";

    // ── Fleet ─────────────────────────────────────────────────────────────────
    ui << " " << BCYN << "YOUR FLEET:" << R << "\n";
    for (const int id : player.getShipIds()) {
        const Entity* e = map.getEntity(id);
        if (!e) continue;

        const Ship* s = e->asShip();
        // Color HP: green if > 50%, yellow if > 25%, red if critical
        std::string_view hp_color = BGRN;
        if (s && s->getMaxHealth() > 0) {
            const int pct = s->getHealth() * 100 / s->getMaxHealth();
            if (pct <= 25) hp_color = BRED;
            else if (pct <= 50) hp_color = BYEL;
        }
        // Color MP: green if full, yellow if partial, red if zero
        std::string_view mp_color = BGRN;
        if (s) {
            if (s->getMovementPoints() == 0) mp_color = BRED;
            else if (s->getMovementPoints() < s->getMovementRange()) mp_color = BYEL;
        }

        ui << "   " << BCYN << e->getName() << R
           << " [" << DGRY << id << R << "]"
           << "  " << hp_color << "HP " << e->getHealth() << "/" << e->getMaxHealth() << R;
        if (s) {
            ui << "  " << YEL << "ATK " << e->getAttackPower() << R
               << "  " << mp_color << "MP " << s->getMovementPoints() << "/" << s->getMovementRange() << R;
        }
        ui << "  " << BYEL << "@" << map.toViewportCoord(e->getPosition()) << R
           << "  " << DGRY << "(sector " << map.sectorOf(e->getPosition()) << ")" << R << "\n";
    }
    ui << DGRY << "----------------------------------------------------------------" << R << "\n";

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

                // Visible contacts grouped by current relation.
                if (e->isAlive() && e->getCivOwner() && e->getCivOwner() != &civ) {
                    if (player.getPerception().isVisible(pos.x, pos.y, map.getWorldWidth())) {
                        anyEnemy = true;
                        enemy_buf << "   " << BRED << "[" << id << "] " << e->getName() << R
                                  << "  " << RED << e->getCivOwner()->getName() << R;
                        if (e->getMaxHealth() > 0) {
                            const int pct = e->getHealth() * 100 / e->getMaxHealth();
                            std::string_view hc = (pct <= 25) ? BRED : (pct <= 50) ? BYEL : BGRN;
                            enemy_buf << "  " << hc << "HP:" << e->getHealth() << "/" << e->getMaxHealth() << R;
                        }
                        enemy_buf << "  " << YEL << "@" << map.toViewportCoord(pos) << R
                                  << "  " << DGRY << "(sector " << map.sectorOf(pos) << ")" << R << "\n";
                    }
                }

                // Known planets
                const Planet* p = e->asPlanet();
                if (p && player.getPerception().isDiscovered(pos.x, pos.y, map.getWorldWidth())) {
                    anyPlanet = true;
                    std::string_view pc = relationBrightTextColor(civ, p->getCivOwner());
                    if (!p->getCivOwner()) {
                        pc = GRN;
                        switch (p->getPlanetType()) {
                            case PlanetType::MINERAL: pc = YEL; break;
                            case PlanetType::ENERGY:  pc = CYN; break;
                            default: break;
                        }
                    }
                    planet_buf << "   " << pc << "[" << id << "] " << p->getName() << R
                               << "  " << BYEL << "@" << map.toViewportCoord(pos) << R
                               << "  " << WHT << "res:" << BGRN << p->getResourceCount() << R
                               << "  " << DGRY << "(sector " << map.sectorOf(pos) << ")" << R;
                    if (p->isColonized())
                        planet_buf << "  " << relationTextColor(civ, p->getCivOwner())
                                   << "[" << p->getCivOwner()->getName() << "]" << R;
                    planet_buf << "\n";
                }
            }
        }
    }

    // Restore viewed sector
    const int sel = selected - 1;
    map.changeSelectedChunk(sel % map.getChunkCols(), sel / map.getChunkCols());

    if (anyEnemy) {
        ui << " " << BRED << "VISIBLE ENEMIES:" << R << "\n" << enemy_buf.str();
        ui << DGRY << "----------------------------------------------------------------" << R << "\n";
    }
    if (anyPlanet) {
        ui << " " << BYEL << "KNOWN PLANETS:" << R << "\n" << planet_buf.str();
        ui << DGRY << "----------------------------------------------------------------" << R << "\n";
    }

    if (!last_message.empty())
        ui << " " << BCYN << ">" << R << " " << WHT << last_message << R
           << "\n" << DGRY << "----------------------------------------------------------------" << R << "\n";

    ui << DGRY << " move <id> <coord>  attack <id> <target>  cancel <id>  mine <id>  research\n";
    ui << " build <fighter|cruiser|transport>  colonize <id>  view <sector>  next  status <id>  quit\n";
    ui << " ▲=Fighter(ATK50 rng1)  ◆=Cruiser(ATK25 rng3,no-retaliation)  ■=Transport  ●=Planet\n";
    ui << "================================================================" << R << "\n";
    const int map_width = VIEWPORT_WIDTH + 4;
    const int panel_col = map_width + 4;
    const int input_row = VIEWPORT_HEIGHT + 3;

    std::istringstream panel(ui.str());
    std::string line;
    for (int row = 1; std::getline(panel, line); ++row) {
        std::cout << "\033[" << row << ";" << panel_col << "H" << line;
    }

    std::cout << "\033[" << input_row << ";1H" << WHT << " Input: " << R;
    std::cout.flush();
}
