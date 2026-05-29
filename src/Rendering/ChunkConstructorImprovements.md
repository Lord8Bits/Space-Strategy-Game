
## Completed
- [✓] Chunk constructor now uses 2D world coordinates (x_start, x_end, y_start, y_end)
- [✓] Map auto-generates chunks in grid layout
- [✓] Entity placement is automatic via Map::addEntity()

## Known Issues / TODOs
- [ ] Ship class implementation missing to thoroughly test Entity abstract class
- [ ] Updatable interface needs integration with TurnManager
- [ ] Entity movement system (setPosition + chunk transfer)