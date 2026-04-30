#pragma once

#include "objects.h"

namespace utils {
    // Patikrina, ar reikšmė yra tarp dviejų ribų
    bool IsBetween(float val, float rangeB, float rangeE);

    // Patikrina, ar žaidėjas yra ant platformos
    bool InOnPlate(const Player& player, const Plate& plate);

    // Palygina score įrašus
    bool CompareScores(const ScoreEntry& a, const ScoreEntry& b);
}
