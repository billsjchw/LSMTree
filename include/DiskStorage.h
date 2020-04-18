#ifndef DISK_STORAGE_H
#define DISK_STORAGE_H

#include "SkipList.h"
#include "LevelZero.h"
#include "LevelNonZero.h"
#include "SearchResult.h"
#include "BlockCache.h"
#include "TableCache.h"
#include <string>
#include <cstdint>
#include <vector>

class DiskStorage {
public:
    explicit DiskStorage(const std::string &dir);
    void add(const SkipList &mem);
    SearchResult search(uint64_t key, bool needValue);
    void clear();
private:
    std::string dir;
    uint64_t no;
    LevelZero level0;
    std::vector<LevelNonZero> levels;
    BlockCache blockCache;
    TableCache tableCache;
    void save() const;
};

#endif