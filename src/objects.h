#pragma once

#include "defines.h"
#include <string>

// ==================== GAME STATE ====================
enum GameState {
    MAIN_MENU,
    PLAYING,
    GAME_OVER,
    ENTERING_NAME,
    LEADERBOARD,
    PAUSE_MENU
};

// ==================== PLAYER ====================
struct Player {
    float x, y; // viršutinio kairiojo kampo koordinatės

    float LegsStartX() const { return x + 10.f; }
    float LegsEndX() const { return x + 40.f; }
    float LegsY() const { return y + 60.f; }
};

// ==================== PLATE ====================
struct Plate {
    float x, y; // viršutinio kairiojo kampo koordinatės

    float StartX() const { return x; }
    float EndX() const { return x + PLATES_WIDTH; }
    float TopY() const { return y; }
    float BottomY() const { return y + PLATES_HEIGHT; }
};

// ==================== SCORE ENTRY ====================
struct ScoreEntry {
    std::string playerName;
    int score;
};

// ==================== GAME SETTINGS ====================
struct GameSettings {
    bool isFullscreen = false;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
};

