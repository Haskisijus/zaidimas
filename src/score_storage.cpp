#include "score_storage.h"
#include <algorithm>
#include <utility>

ScoreStorage::ScoreStorage() : entries(nullptr), count(0), capacity(0) {}

ScoreStorage::ScoreStorage(std::size_t initialCapacity)
    : entries(nullptr), count(0), capacity(0) {
    if (initialCapacity > 0) {
        reserve(initialCapacity);
    }
}

ScoreStorage::ScoreStorage(const ScoreStorage& other)
    : entries(nullptr), count(other.count), capacity(other.capacity) {
    if (capacity > 0) {
        entries = new ScoreEntry[capacity];
        std::copy(other.entries, other.entries + count, entries);
    }
}

ScoreStorage& ScoreStorage::operator=(const ScoreStorage& other) {
    if (this == &other) {
        return *this;
    }

    ScoreEntry* newEntries = nullptr;
    if (other.capacity > 0) {
        newEntries = new ScoreEntry[other.capacity];
        std::copy(other.entries, other.entries + other.count, newEntries);
    }

    delete[] entries;
    entries = newEntries;
    count = other.count;
    capacity = other.capacity;
    return *this;
}

ScoreStorage::~ScoreStorage() {
    delete[] entries;
    entries = nullptr;
    count = 0;
    capacity = 0;
}

void ScoreStorage::reserve(std::size_t newCapacity) {
    if (newCapacity <= capacity) {
        return;
    }

    ScoreEntry* newEntries = new ScoreEntry[newCapacity];
    for (std::size_t i = 0; i < count; ++i) {
        newEntries[i] = entries[i];
    }

    delete[] entries;
    entries = newEntries;
    capacity = newCapacity;
}

void ScoreStorage::add(const ScoreEntry& entry) {
    if (count == capacity) {
        reserve(capacity == 0 ? 4 : capacity * 2);
    }
    entries[count++] = entry;
}

void ScoreStorage::clear() {
    count = 0;
}

void ScoreStorage::sortDescending() {
    if (count < 2 || entries == nullptr) {
        return;
    }
    std::sort(entries, entries + count);
}

std::size_t ScoreStorage::size() const {
    return count;
}

const ScoreEntry* ScoreStorage::data() const {
    return entries;
}

std::vector<ScoreEntry> ScoreStorage::toVector() const {
    if (count == 0 || entries == nullptr) {
        return {};
    }
    return std::vector<ScoreEntry>(entries, entries + count);
}


