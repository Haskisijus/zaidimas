#pragma once

#include "objects.h"
#include <vector>
#include <string>

class LeaderboardManager {
private:
    std::vector<ScoreEntry> leaderboard;

public:
    void Load();
    void Save();
    void AddScore(const std::string& name, int score);
    const std::vector<ScoreEntry>& GetLeaderboard() const;
    std::string GetFormattedLeaderboard(int maxEntries = 8) const;
};

