#include "leaderboard_manager.h"
#include "utils.h"
#include "defines.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

void LeaderboardManager::Load() {
    leaderboard.clear();
    std::ifstream file(LEADERBOARD_FILE);
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                size_t delimPos = line.rfind(':');
                if (delimPos != std::string::npos) {
                    std::string name = line.substr(0, delimPos);
                    int scoreVal = stoi(line.substr(delimPos + 1));
                    leaderboard.add({name, scoreVal});
                }
            }
        }
        file.close();
        leaderboard.sortDescending();
    }
}

void LeaderboardManager::Save() {
    leaderboard.sortDescending();
    std::ofstream file(LEADERBOARD_FILE);
    if (file.is_open()) {
        const auto entries = leaderboard.toVector();
        for (const auto& entry : entries) {
            file << entry.playerName << ":" << entry.score << "\n";
        }
        file.close();
    }
}

void LeaderboardManager::AddScore(const std::string& name, int score) {
    leaderboard.add({name, score});
    Save();
}

std::vector<ScoreEntry> LeaderboardManager::GetLeaderboard() const {
    return leaderboard.toVector();
}

std::string LeaderboardManager::GetFormattedLeaderboard(int maxEntries) const {
    std::ostringstream result;
    ScoreStorage snapshot = leaderboard;
    snapshot.sortDescending();
    int count = 0;
    const auto entries = snapshot.toVector();
    for (const auto& entry : entries) {
        if (count >= maxEntries) break;
        result << entry << "\n";
        count++;
    }
    if (count == 0) {
        return "No scores yet!";
    }
    return result.str();
}

