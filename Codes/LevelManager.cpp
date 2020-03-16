#include "LevelManager.h"
#include "SSTableController.h"
#include <sstream>

LevelManager::LevelManager(const std::string &dir)
    : dir(dir), sstCnt(0), byteCnt(0) {}

LevelManager::~LevelManager() {
    for (SSTableIndex *index : indices)
        delete index;
}

std::string LevelManager::get(uint64_t key, bool &success) {
    success = false;
    std::string ret;
    for (uint64_t i = 1; i <= sstCnt; ++i) {
        uint64_t k = indices[sstCnt - i]->find(key, success);
        if (success) {
            SSTableController ctrl;
            ctrl.open(name(files[i]), indices[i]);
            ret = ctrl.get(k);
            ctrl.close();
            break;
        }
    }
    return ret;
}

void LevelManager::add(uint64_t file, SSTableIndex *index) {
    files.push_back(file);
    indices.push_back(index);
    sstCnt += 1;
    byteCnt += index->space();
}

std::string LevelManager::name(uint64_t file) {
    std::ostringstream out;
    out << dir << "/" << file << ".sst";
    return out.str();
}