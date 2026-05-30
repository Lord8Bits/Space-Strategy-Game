# Space Strategy Game

A turn-based console space strategy game developed for the **S4 OOP C++ mini-project** (Licence en Génie Informatique). Players command a space civilization, manage a fleet, exploit planet resources, and battle enemy factions across a multi-sector grid.

---

## Requirements

| Tool | Minimum version |
|------|----------------|
| C++ compiler with C++23 support | GCC 13+ or Clang 16+ |
| CMake | 3.20+ |
| Git | any recent version |
| Terminal | UTF-8 + ANSI color support (Linux/macOS native; Windows: use Windows Terminal) |

---

## Build & Run

```bash
# 1. Clone the repository
git clone https://github.com/<your-org>/Space-Strategy-Game.git
cd Space-Strategy-Game

# 2. Configure with CMake
cmake -B build

# 3. Compile
cmake --build build

# 4. Run
./build/Space_Game
```

---

## Gameplay Overview

The game world is divided into **6 sectors** arranged in a 2×3 grid (each sector is 80×20 cells). Your civilization starts in **sector 1** with a home planet (Terra), two combat ships, and a transport.

### Coordinate system

Positions are **viewport-relative** to whichever sector you are currently viewing:
- **X axis** — numbers 1–80 shown at the top of the map
- **Y axis** — letters A–T shown on the left of the map

Example: `A1` = top-left corner of the current sector, `T80` = bottom-right.

Use `view <sector>` to switch sectors before issuing movement orders.

---

## Commands

| Command | Description |
|---------|-------------|
| `move <id> <coord>` | Move ship to viewport coordinate, e.g. `move 1 J20` |
| `attack <id> <target_id>` | Attack an enemy ship (costs all movement points) |
| `colonize <transport_id>` | Colonize the planet your Transport is standing on |
| `mine <transport_id>` | Extract resources from the planet your Transport is on |
| `build <fighter\|cruiser\|transport>` | Build a ship at your home planet (Terra) |
| `research` | Upgrade Plasma Cannons (boosts Fighter attack power) |
| `view <sector>` | Switch the viewed sector (1–6) |
| `next` | End your turn — ships travel, enemy AI acts |
| `status <id>` | Show detailed info for any entity |
| `help` | List all commands |
| `quit` | Exit the game |

### Ship types

| Symbol | Type | Role |
|--------|------|------|
| ▲ | Fighter | Fast combat ship — dodge chance, benefits from Plasma Cannons research |
| ◆ | Cruiser | Heavy ship — absorbs damage with a shield, counter-attacks |
| ■ | Transport | Non-combat — mines planets, colonizes, carries resources |

### Planet types

| Symbol | Color | Type | Resource yield |
|--------|-------|------|----------------|
| ● | Green | Terrain | Balanced |
| ● | Yellow | Mineral | Titanium-rich |
| ● | Cyan | Energy | Cadmium-rich |

---

## OOP Concepts Used

| Concept | Where |
|---------|-------|
| **Inheritance** | `Entity` → `Ship` → `Fighter`, `Cruiser`, `Transport`; `Entity` → `Planet` |
| **Polymorphism** | Virtual dispatch for combat (`tryDodge`, `absorbDamage`, `canCounterAttack`), rendering (`getSymbol`, `getDisplayColor`), AI (`takeTurn`) |
| **Operator overloads** | `Resource`: `+`, `-`, `+=`, `-=`, `>=`, `<`, `==`, `!=`, `<<` |
| **Friend functions** | `operator<<(ostream&, const Resource&)` — direct access to private fields |
| **Encapsulation** | Private members with getters/setters throughout |

---

## Team & Contributions

| Member | Responsibilities |
|--------|-----------------|
| **Aymane Larhrissi** *(Lead)* | Spatial partitioning (`Map`, `Chunk`), rendering engine (`Render`, `SubjectiveRender`), `Game` class architecture, AI system, colonization mechanic, overall integration |
| **Ilyas** (FengMin-000) | Ship hierarchy (`Ship`, `Fighter`, `Cruiser`, `Transport`), XP/level system, action framework |
| **Yassir Hikou** | `Resource` class and operators, `Planet` class, `Civilization` system |
| **Ilyas Chatir** | `TurnManager`, `ViewPort` coordinate translation, `InputParser` |
| **Reda Gachouch** | `Player` state management, `Perception` fog-of-war (bitset), `SubjectiveRender` filtering |

---

## Project Structure

```
Space-Strategy-Game/
├── include/          # All class headers (.hpp)
├── src/
│   ├── AI/           # Civilization AI (takeTurn logic)
│   ├── Entities/     # Ship, Fighter, Cruiser, Transport, Planet
│   ├── Rendering/    # Map, Render, SubjectiveRender, Chunk, ViewPort
│   ├── Systems/      # CombatSystem, Resource, Technology
│   ├── Utils/        # Constants, Enums, Position, Action
│   ├── Game.cpp      # Top-level game loop and command handling
│   └── main.cpp      # Entry point
└── CMakeLists.txt
```
