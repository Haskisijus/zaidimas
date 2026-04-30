#include "leaderboard_manager.h"
#include "utils.h"
#include "defines.h"
#include <fstream>
#include <sstream>
#include <algorithm>

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
                    leaderboard.push_back({name, scoreVal});
                }
            }
        }
        file.close();
        sort(leaderboard.begin(), leaderboard.end(), utils::CompareScores);
    }
}

void LeaderboardManager::Save() {
    sort(leaderboard.begin(), leaderboard.end(), utils::CompareScores);
    std::ofstream file(LEADERBOARD_FILE);
    if (file.is_open()) {
        for (const auto& entry : leaderboard) {
            file << entry.playerName << ":" << entry.score << "\n";
        }
        file.close();
    }
}

void LeaderboardManager::AddScore(const std::string& name, int score) {
    leaderboard.push_back({name, score});
    Save();
}

const std::vector<ScoreEntry>& LeaderboardManager::GetLeaderboard() const {
    return leaderboard;
}

std::string LeaderboardManager::GetFormattedLeaderboard(int maxEntries) const {
    std::string result = "";
    int count = 0;
    for (const auto& entry : leaderboard) {
        if (count >= maxEntries) break;
        result += entry.playerName + " - " + std::to_string(entry.score) + "\n";
        count++;
    }
    if (result.empty()) {
        result = "No scores yet!";
    }
    return result;
}

