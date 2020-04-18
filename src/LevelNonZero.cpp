#include "LevelNonZero.h"
#include "Util.h"
#include "SSTableId.h"
#include <filesystem>
#include <fstream>

LevelNonZero::LevelNonZero(const std::string &dir, TableCache *tableCache): dir(dir), tableCache(tableCache) {
    if (!std::filesystem::exists(std::filesystem::path(dir))) {
        std::filesystem::create_directories(std::filesystem::path(dir));
        size = 0;
        byteCnt = 0;
        lastKey = 0;
        save();
    } else {
        std::ifstream ifs(dir + "/index", std::ios::binary);
        ifs.read((char*) &size, sizeof(uint64_t));
        ifs.read((char*) &byteCnt, sizeof(uint64_t));
        ifs.read((char*) &lastKey, sizeof(uint64_t));
        for (uint64_t i = 0; i < size; ++i) {
            uint64_t no;
            ifs.read((char*) &no, sizeof(uint64_t));
            ssts.emplace_back(SSTableId(dir, no), tableCache);
        }
        ifs.close();
    }
}

SearchResult LevelNonZero::search(uint64_t key) const {
    for (const SSTable &sst : ssts) {
        SearchResult res = sst.search(key);
        if (res.success)
            return res;
    }
    return false;
}

std::vector<Entry> LevelNonZero::extract() {
    auto itr = ssts.begin();
    while (itr != ssts.end() && itr->upper() <= lastKey)
        ++itr;
    if (itr == ssts.end())
        itr = ssts.begin();
    byteCnt -= itr->space();
    lastKey = itr->upper();
    std::vector<Entry> ret = itr->load();
    itr->remove();
    ssts.erase(itr);
    --size;
    save();
    return ret;
}

void LevelNonZero::merge(std::vector<Entry> &&entries1, uint64_t &no) {
    uint64_t lo = entries1[0].key;
    uint64_t hi = entries1.back().key;
    std::vector<Entry> entries0;
    auto itr = ssts.begin();
    while (itr != ssts.end() && itr->upper() < lo)
        ++itr;
    while (itr != ssts.end() && itr->lower() <= hi) {
        for (const Entry &entry : itr->load())
            entries0.emplace_back(entry);
        byteCnt -= itr->space();
        itr->remove();
        itr = ssts.erase(itr);
        --size;
    }
    std::vector<Entry> entries = Util::compact({entries0, entries1});
    size_t n = entries.size();
    size_t pos = 0;
    while (pos < n) {
        byteCnt += ssts.emplace(itr, entries, pos, SSTableId(dir, no++), tableCache)->space();
        ++size;
    }
    save();
}

void LevelNonZero::clear() {
    while (!ssts.empty()) {
        ssts.back().remove();
        ssts.pop_back();
    }
    size = 0;
    byteCnt = 0;
    lastKey = 0;
    save();
}

uint64_t LevelNonZero::space() const {
    return byteCnt;
}

void LevelNonZero::save() const {
    std::ofstream ofs(dir + "/index", std::ios::binary);
    ofs.write((char*) &size, sizeof(uint64_t));
    ofs.write((char*) &byteCnt, sizeof(uint64_t));
    ofs.write((char*) &lastKey, sizeof(uint64_t));
    for (const SSTable &sst : ssts) {
        uint64_t no = sst.number();
        ofs.write((char*) &no, sizeof(uint64_t));
    }
    ofs.close();
}