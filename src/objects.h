#pragma once

#include "defines.h"
#include <ostream>
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

// ==================== PLATE TYPE ====================
enum PlateType {
    NORMAL_PLATE,   // Žalia - paprastoji
    FUEL_PLATE,     // Geltona - duoda kurą
    DANGER_PLATE    // Raudona - game over
};

// ==================== PLATE ====================
struct Plate {
    float x, y; // viršutinio kairiojo kampo koordinatės
    PlateType type = NORMAL_PLATE;
    int fuelAmount = 0;  // Kuro kiekis FUEL_PLATE type'ui (50-100%)

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

inline bool operator<(const ScoreEntry& a, const ScoreEntry& b) {
    return a.score > b.score;
}

inline std::ostream& operator<<(std::ostream& os, const ScoreEntry& entry) {
    os << entry.playerName << " - " << entry.score;
    return os;
}

// ==================== GAME SETTINGS ====================
struct GameSettings {
    bool isFullscreen = false;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
};

