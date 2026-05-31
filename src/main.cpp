#include "Game.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

static void platformInit() {
#ifdef _WIN32
    // Enable UTF-8 output codepage so Unicode symbols (▲◆■●) render correctly
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    // Enable ANSI virtual terminal processing so \x1b[...m color codes work
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode))
            SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

int main() {
    platformInit();
    Game game;
    game.run();
    return 0;
}
