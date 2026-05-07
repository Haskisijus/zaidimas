#pragma once

#include "objects.h"
#include "score_storage.h"
#include <string>

class LeaderboardManager {
private:
    ScoreStorage leaderboard;

public:
    void Load();
    void Save();
    void AddScore(const std::string& name, int score);
    std::vector<ScoreEntry> GetLeaderboard() const;
    std::string GetFormattedLeaderboard(int maxEntries = 8) const;
};

