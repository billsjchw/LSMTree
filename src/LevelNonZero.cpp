#include "LevelNonZero.h"
#include "Util.h"
#include "SSTableId.h"
#include <filesystem>
#include <fstream>

LevelNonZero::LevelNonZero(const std::string &dir): dir(dir) {
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
            ssts.emplace_back(SSTableId(dir, no));
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
#include <iostream>
std::vector<Entry> LevelNonZero::extract() {
    std::cout << ssts.size() << std::endl;
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

void LevelNonZero::merge(std::vector<Entry> &&lData, uint64_t &no) {
    uint64_t lo = lData[0].key;
    uint64_t hi = lData.back().key;
    std::vector<Entry> eData;
    auto itr = ssts.begin();
    while (itr != ssts.end() && itr->upper() < lo)
        ++itr;
    while (itr != ssts.end() && itr->lower() <= hi) {
        for (const Entry &entry : itr->load())
            eData.emplace_back(entry);
        byteCnt -= itr->space();
        itr->remove();
        itr = ssts.erase(itr);
        --size;
    }
    std::vector<Entry> data = Util::compact({eData, lData});
    size_t n = data.size();
    size_t pos = 0;
    while (pos < n) {
        byteCnt += ssts.emplace(itr, data, pos, SSTableId(dir, no++))->space();
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