#include "utils.h"

bool utils::IsBetween(float val, float rangeB, float rangeE) {
    return val > rangeB && val < rangeE;
}

bool utils::InOnPlate(const Player& player, const Plate& plate) {
    const bool betweenX = IsBetween(player.LegsStartX(), plate.StartX(), plate.EndX()) ||
        IsBetween(player.LegsEndX(), plate.StartX(), plate.EndX());

    const bool betweenY = IsBetween(player.LegsY(), plate.TopY(), plate.BottomY());

    return betweenX && betweenY;
}

bool utils::CompareScores(const ScoreEntry& a, const ScoreEntry& b) {
    return a.score > b.score;
}

