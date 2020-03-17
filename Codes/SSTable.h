#ifndef SSTABLE_H
#define SSTABLE_H

#include "SkipList.h"
#include "Entry.h"
#include "SearchResult.h"
#include "SSTableId.h"
#include <string>
#include <vector>
#include <cstdint>
#include <cstddef>

class SSTable {
public:
    explicit SSTable(const SSTableId &id);
    explicit SSTable(const SkipList &memTable, const SSTableId &id);
    explicit SSTable(const std::vector<Entry> &data, size_t &pos, uint64_t bound, const SSTableId &id);
    SearchResult search(uint64_t key) const;
    std::vector<Entry> load() const;
    void remove() const;
    uint64_t number() const;
    uint64_t lower() const;
    uint64_t upper() const;
private:
    SSTableId id;
    uint64_t size;
    std::vector<uint64_t> keys;
    std::vector<uint64_t> offsets;
    void save(const std::string &values) const;
    std::string read(uint64_t pos) const;
};

#endif