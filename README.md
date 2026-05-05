# Space Strategy Game: Development Roadmap

## Team Responsibilities

### Core Logic & Entities
**Ilyas** (FengMin-000)
*   **Ship Hierarchy**: Development of the base `Ship` class and specialized subclasses (`DestroyerShip`, `CruiserShip`, `MinerShip`).
*   **Skill System**: Implementation of a numerical ID-based skill mechanic.
*   **Action Framework**: Creation of a modular action system using IDs to allow for scalable ship capabilities.
*   **Documentation**: Maintenance of a master spreadsheet mapping all Skill and Action IDs.

**Yassir Hikou**
*   **Resource Management**: Implementation of the `Resource` class to handle various types across different containers like Planets and Civilizations.
*   **Celestial Bodies**: Development of the `Planet` class including resource exploitation logic and state flags.
*   **Civilization System**: Engineering the `Civilization` class to manage player construction, research, and high-level actions.

### Game Engine & Interface
**Ilyas Chatir**
*   **Turn Management**: Implementation of the `TurnManager` to process and synchronize player actions at the end of each cycle.
*   **Coordinate Translation**: Creation of the `ViewPort` system to map input coordinates to internal memory indices.
*   **Input Parsing**: Development of the `InputParser` to translate user strings into executable `SpaceEntity` actions.

**Aymane Larhrissi (Lead)**
*   **Spatial Partitioning**: Development of the `Sector` class to manage local $(x, y)$ coordinate ranges.
*   **World Mapping**: Creation of the `Map` class to track all sectors and manage the current active player view.
*   **Rendering Engine**: Implementation of the high-performance `Render` system to draw `SpaceEntity` data (position, symbols, colors) at 60 FPS.

---

## Technical Guidelines

To ensure code maintainability and minimize technical debt, all contributors must adhere to the following standards:

### Naming Conventions
| Element | Convention | Example |
| :--- | :--- | :--- |
| **Classes** | `PascalCase` | `InputParser` |
| **Methods** | `camelCase` | `getPos()` |
| **Variables/Attributes** | `snake_case` | `move_pts` |
| **Private Attributes** | `_leading_snake_case` | `_max_hp` |
| **Constants** | `SCREAMING_SNAKE_CASE` | `MAX_WIDTH` |

### Coding Standards

> **Zero Tolerance for Magic Numbers**
> Numbers with non-obvious significance (e.g., `300`) must be replaced with a `static constexpr`. This ensures type safety and centralizes game balance variables.

*   **Language**: All code (variables, functions, classes) must be written in **English**.
*   **Clarity**: Avoid ambiguous names (e.g., `getC()`, `w`). Use descriptive identifiers. Coordinate variables `x` and `y` are the only exceptions.
*   **File Structure**: Strict separation of concerns.
    *   **Header Files (.hpp)**: Declarations of classes, functions, and structs.
    *   **Source Files (.cpp)**: Implementation/Definitions of logic.
