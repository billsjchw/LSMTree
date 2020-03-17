#ifndef SSTABLE_H
#define SSTABLE_H

#include "SkipList.h"
#include "Entry.h"
#include "SearchResult.h"
#include <string>
#include <vector>
#include <cstdint>

class SSTable {
public:
    explicit SSTable(const std::string &filename);
    explicit SSTable(const SkipList &memTable, const std::string &filename);
    explicit SSTable(const std::vector<Entry> &data, uint64_t &pos, uint64_t bound, const std::string &filename);
    SearchResult search(uint64_t key);
    std::vector<Entry> load();
    void remove();
private:
    std::string filename;
    uint64_t size;
    std::vector<uint64_t> keys;
    std::vector<uint64_t> offsets;
    void save(const std::string &values);
    std::string read(uint64_t pos);
};

#endif