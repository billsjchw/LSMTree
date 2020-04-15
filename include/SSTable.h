#ifndef SSTABLE_H
#define SSTABLE_H

#include "SkipList.h"
#include "Entry.h"
#include "SearchResult.h"
#include "SSTableId.h"
#include "Location.h"
#include <string>
#include <vector>
#include <cstdint>
#include <cstddef>

class SSTable {
public:
    explicit SSTable(const SSTableId &id);
    explicit SSTable(const SkipList &mem, const SSTableId &id);
    explicit SSTable(const std::vector<Entry> &entries, size_t &pos, const SSTableId &id);
    SearchResult search(uint64_t key) const;
    std::vector<Entry> load() const;
    std::string loadBlock(uint64_t pos) const;
    void remove() const;
    uint64_t number() const;
    uint64_t lower() const;
    uint64_t upper() const;
    uint64_t space() const;
private:
    SSTableId id;
    uint64_t entryCnt;
    uint64_t blockCnt;
    std::vector<uint64_t> keys;
    std::vector<uint64_t> offsets;
    std::vector<uint64_t> oris;
    std::vector<uint64_t> cmps;
    void save(const std::string &blockSeg);
    Location locate(uint64_t pos) const;
    uint64_t indexSpace() const;
    uint64_t blockSpace() const;
};

#endif
