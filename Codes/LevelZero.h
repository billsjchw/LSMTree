#ifndef LEVEL_ZERO_H
#define LEVEL_ZERO_H

#include "SSTable.h"
#include "SkipList.h"
#include "SearchResult.h"
#include "Entry.h"
#include <string>
#include <cstdint>
#include <vector>

class LevelZero {
public:
    explicit LevelZero(const std::string &dir);
    ~LevelZero();
    SearchResult search(uint64_t key) const;
    void add(const SkipList &memTable, uint64_t &no);
    std::vector<Entry> extract();
    void clear();
    uint64_t space() const;
private:
    std::string dir;
    uint64_t size;
    uint64_t byteCnt;
    std::vector<SSTable> ssTables;
};

#endif