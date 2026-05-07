#pragma once

#include <string>

// Ekrano nustatymai
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 533;

// Žaidėjo parametrai
const float PLAYER_WIDTH = 50.f;
const float MAX_PLAYER_Y = 200.f;

// Platformų parametrai
const int PLATES_AMOUNT = 6;  // Sumažintai - platformos retesnės
const float PLATES_WIDTH = 68.f;
const float PLATES_HEIGHT = 14.f;

// Kuro parametrai
const float MAX_FUEL = 100.0f;

// Failai
const std::string LEADERBOARD_FILE = "leaderboard.txt";

// Komentarinimas skirti testui
//#define SOUND_ON
