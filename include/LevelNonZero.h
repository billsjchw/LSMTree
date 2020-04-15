#ifndef LEVEL_NON_ZERO_H
#define LEVEL_NON_ZERO_H

#include "SSTable.h"
#include "SearchResult.h"
#include <string>
#include <cstdint>
#include <vector>
#include <list>

class LevelNonZero {
public:
    explicit LevelNonZero(const std::string &dir);
    SearchResult search(uint64_t key) const;
    std::vector<Entry> extract();
    void merge(std::vector<Entry> &&entries1, uint64_t &no);
    void clear();
    uint64_t space() const;
private:
    std::string dir;
    uint64_t size;
    uint64_t byteCnt;
    uint64_t lastKey;
    std::list<SSTable> ssts;
    void save() const;
};

#endif