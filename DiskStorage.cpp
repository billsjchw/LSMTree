#include "DiskStorage.h"
#include "Utility.h"
#include <filesystem>
#include <fstream>
#include <iostream>

DiskStorage::DiskStorage(const std::string &dir): dir(dir), level0(dir + Utility::LEVEL_ZERO_NAME) {
    if (std::filesystem::exists(std::filesystem::path(dir + "/meta"))) {
        std::ifstream ifs(dir + "/meta", std::ios::binary);
        ifs.read((char*) &no, sizeof(uint64_t));
        ifs.close();
    } else {
        no = 0;
        save();
    }
    for (uint64_t i = 0; i < Utility::LEVEL_NON_ZERO_NUM; ++i)
        levels.emplace_back(dir + Utility::LEVEL_NON_ZERO_NAMES[i]);
}

void DiskStorage::add(const SkipList &memTable) {
    level0.add(memTable, no);
    if (level0.space() > Utility::LEVEL_ZERO_BOUND)
        levels[0].merge(level0.extract(), no);
    for (uint64_t i = 0; i + 1 < Utility::LEVEL_NON_ZERO_NUM; ++i)
        if (levels[i].space() > Utility::LEVEL_NON_ZERO_BOUNDS[i])
            levels[i + 1].merge(levels[i].extract(), no);
    save();
}

SearchResult DiskStorage::search(uint64_t key) {
    SearchResult res = level0.search(key);
    for (uint64_t i = 0; !res.success && i < Utility::LEVEL_NON_ZERO_NUM; ++i)
        res = levels[i].search(key);
    return res;
}

void DiskStorage::clear() {
    level0.clear();
    for (uint64_t i = 0; i < Utility::LEVEL_NON_ZERO_NUM; ++i)
        levels[i].clear();
    no = 0;
    save();
}

void DiskStorage::save() const {
    std::ofstream ofs(dir + "/meta", std::ios::binary);
    ofs.write((char*) &no, sizeof(uint64_t));
    ofs.close();
}