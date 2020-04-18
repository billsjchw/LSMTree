#include "DiskStorage.h"
#include "Util.h"
#include "Option.h"
#include <filesystem>
#include <fstream>

DiskStorage::DiskStorage(const std::string &dir): dir(dir), level0(dir + Option::Z_NAME, &tableCache) {
    if (std::filesystem::exists(std::filesystem::path(dir + "/meta"))) {
        std::ifstream ifs(dir + "/meta", std::ios::binary);
        ifs.read((char*) &no, sizeof(uint64_t));
        ifs.close();
    } else {
        no = 0;
        save();
    }
    for (uint64_t i = 0; i < Option::NZ_NUM; ++i)
        levels.emplace_back(dir + Option::NZ_NAMES[i], &tableCache);
}

void DiskStorage::add(const SkipList &mem) {
    level0.add(mem, no);
    if (Option::COMPACTION) {
        if (level0.space() > Option::Z_SPACE)
            levels[0].merge(level0.extract(), no);
        for (uint64_t i = 0; i + 1 < Option::NZ_NUM; ++i)
            if (levels[i].space() > Option::NZ_SPACES[i])
                levels[i + 1].merge(levels[i].extract(), no);
    }
    save();
}

SearchResult DiskStorage::search(uint64_t key, bool needValue) {
    SearchResult result = level0.search(key);
    for (uint64_t i = 0; !result.success && i < Option::NZ_NUM; ++i)
        result = levels[i].search(key);
    if (needValue)
        blockCache.complete(result);
    return result;
}

void DiskStorage::clear() {
    level0.clear();
    for (uint64_t i = 0; i < Option::NZ_NUM; ++i)
        levels[i].clear();
    no = 0;
    save();
}

void DiskStorage::save() const {
    std::ofstream ofs(dir + "/meta", std::ios::binary);
    ofs.write((char*) &no, sizeof(uint64_t));
    ofs.close();
}
