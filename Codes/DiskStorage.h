#ifndef DISK_STORAGE_H
#define DISK_STORAGE_H

#include "SkipList.h"
#include "LevelZero.h"
#include "LevelNonZero.h"
#include <string>
#include <cstdint>
#include <vector>

class DiskStorage {
public:
    explicit DiskStorage(const std::string &dir);
    ~DiskStorage();
    void add(const SkipList &memTable);
    std::string get(uint64_t key);
private:
    std::string dir;
    uint64_t no;
    LevelZero level0;
    std::vector<LevelNonZero> levels;
};

#endif