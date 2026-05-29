# Rendering System Checklist

## Core Functionality (MUST HAVE)
- [✓] Entity renders with correct symbol
- [✓] Entity renders with correct color
- [✓] Multiple entities render correctly
- [✓] main.cpp compiles and runs without errors

## Chunk System (MUST HAVE)
- [✓] Chunk stores entities by ID
- [✓] addEntity() works
- [✓] removeEntity() works
- [✓] Multiple chunks work

## Map System (MUST HAVE)
- [✓] Map stores multiple chunks
- [✓] Map tracks selected chunk
- [✓] Map can add chunks

## Next Goals (NICE TO HAVE)
- [ ] Optimized rendering (dirty rectangles)
- [ ] Entity movement
- [ ] Wraparound at screen edges

## Do NOT touch until above are done:
- [ ] Coordinate system optimization
- [ ] Data structure refactoring
- [ ] Performance tuning
