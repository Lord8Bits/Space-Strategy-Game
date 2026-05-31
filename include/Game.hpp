#pragma once

#include "Civilization.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "CombatSystem.hpp"
#include "Render.hpp"
#include "SubjectiveRender.hpp"
#include "InputParser.hpp"
#include <string>

/// @brief Top-level game object.  Owns all game state and contains the REPL loop.
///
/// Declaration order = member initialisation order — do not reorder members
/// without also reordering the initialiser list in Game.cpp.
class Game {
    // ── Civilizations first — entities hold pointers to these ─────────────────
    Civilization   _player_civ;
    Civilization   _enemy_civ;

    // ── World and systems ─────────────────────────────────────────────────────
    Map            _map;
    Player         _player;
    CombatSystem   _combat;

    // ── Rendering — Render must precede SubjectiveRender (it holds a Render&) ─
    Render         _renderer;
    SubjectiveRender _sr;

    // ── Input — needs map size, so after Map ─────────────────────────────────
    InputParser    _parser;

    // ── Game state ────────────────────────────────────────────────────────────
    int  _turn         = 1;
    int  _ship_count   = 0;     ///< Tracks built ships for naming (F-4, C-5, …)
    int  _home_id      = -1;    ///< Entity ID of the player's home planet
    bool _devFogEnabled = false; ///< Dev mode: reveal entire world until toggled off

public:
    Game();
    void run();

private:
    void        initWorld();
    std::string executeAction(const Action& a);
    void        advanceTurn();
    void        applyVision();  ///< refreshFogOfWar + re-apply devfog if enabled
};
