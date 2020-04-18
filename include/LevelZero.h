#ifndef LEVEL_ZERO_H
#define LEVEL_ZERO_H

#include "SSTable.h"
#include "SkipList.h"
#include "SearchResult.h"
#include "Entry.h"
#include "TableCache.h"
#include <string>
#include <cstdint>
#include <vector>

class LevelZero {
public:
    explicit LevelZero(const std::string &dir, TableCache *tableCache);
    SearchResult search(uint64_t key) const;
    void add(const SkipList &mem, uint64_t &no);
    std::vector<Entry> extract();
    void clear();
    uint64_t space() const;
private:
    std::string dir;
    uint64_t size;
    uint64_t byteCnt;
    std::vector<SSTable> ssts;
    TableCache *tableCache;
    void save() const;
};

#endif