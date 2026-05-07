#pragma once

#include "objects.h"
#include <cstddef>
#include <vector>

class ScoreStorage {
private:
    ScoreEntry* entries;
    std::size_t count;
    std::size_t capacity;

    void reserve(std::size_t newCapacity);

public:
    ScoreStorage();
    explicit ScoreStorage(std::size_t initialCapacity);
    ScoreStorage(const ScoreStorage& other);
    ScoreStorage& operator=(const ScoreStorage& other);
    ~ScoreStorage();

    void add(const ScoreEntry& entry);
    void clear();
    void sortDescending();

    std::size_t size() const;
    const ScoreEntry* data() const;
    std::vector<ScoreEntry> toVector() const;
};

