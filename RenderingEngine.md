# Render System Design

## What I'm Building
A terminal-based rendering engine that displays entities in a chunked world.

## How It Works (One Sentence Each)
1. **Chunk**: Spatial partition that stores entities by ID in a map
2. **Map**: Container of chunks, tracks which chunk is selected
3. **Render**: Fills a world buffer with entity data, outputs to terminal

## Coordinate System (for now)
- Entity._pos: Absolute world coordinates (X, Y)
- Chunk._idx_start/end: 1D indices in the world buffer
- Viewport: Always 80×20 cells

## Conversion Formula
`(y * VIEWPORT_WIDTH + x) + selected_sector * (VIEWPORT_HEIGHT*VIEWPORT_WIDTH)`
