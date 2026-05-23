# Render System Design

## What I Built
A terminal-based rendering engine that displays entities in a chunked world at 27,000+ FPS.

## Architecture
1. **Chunk**: Spatial partition storing entities by ID in std::map (O(log n) lookup)
2. **Map**: Grid of chunks with O(1) spatial lookup via division
3. **Render**: Uses ViewPort for coordinate conversion, renders one chunk at a time

## Coordinate System
- **Entity position**: Absolute world coordinates (Vec2)
- **Chunk bounds**: 2D world coordinates (_x_start, _x_end, _y_start, _y_end)
- **Viewport**: 80×20 cells, represents one chunk
- **ViewPort class**: Handles world ↔ local coordinate conversions

## Spatial Lookup (O(1))
```cpp
chunk_col = world_x / VIEWPORT_WIDTH;
chunk_row = world_y / VIEWPORT_HEIGHT;
chunk_index = chunk_row * num_cols + chunk_col;
```