#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "SSTableIndex.h"
#include <cstdint>
#include <vector>
#include <string>

class LevelManager {
public:
    explicit LevelManager(const std::string &dir);
    ~LevelManager();
    std::string get(uint64_t key, bool &success);
    void add(uint64_t file, SSTableIndex *index);
private:
    std::string dir;
    bool levelZero;
    uint64_t sstCnt;
    uint64_t byteCnt;
    std::vector<uint64_t> files;
    std::vector<SSTableIndex*> indices;
    std::string name(uint64_t file);
};

#endif