#include "LevelNonZero.h"
#include "Utility.h"
#include "SSTableId.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <new>

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
            ssTables.emplace_back(SSTableId(dir, no));
        }
        ifs.close();
    }
}

SearchResult LevelNonZero::search(uint64_t key) const {
    for (const SSTable &ssTable : ssTables) {
        SearchResult res = ssTable.search(key);
        if (res.success)
            return res;
    }
    return {false, ""};
}

std::vector<Entry> LevelNonZero::extract() {
    auto itr = ssTables.begin();
    while (itr != ssTables.end() && itr->upper() <= lastKey)
        ++itr;
    if (itr == ssTables.end())
        itr = ssTables.begin();
    byteCnt -= itr->space();
    lastKey = itr->upper();
    std::vector<Entry> ret = itr->load();
    itr->remove();
    ssTables.erase(itr);
    --size;
    save();
    return ret; 
}

void LevelNonZero::merge(std::vector<Entry> &&lData, uint64_t &no) {
    uint64_t lo = lData[0].key;
    uint64_t hi = lData.back().key;
    std::vector<Entry> eData;
    auto itr = ssTables.begin();
    while (itr != ssTables.end() && itr->upper() < lo)
        ++itr;
    while (itr != ssTables.end() && itr->lower() <= hi) {
        for (Entry &entry : itr->load())
            eData.emplace_back(std::move(entry));
        byteCnt -= itr->space();
        itr->remove();
        itr = ssTables.erase(itr);
        --size;
    }
    std::vector<Entry> data = Utility::compact({std::move(eData), lData});
    size_t n = data.size();
    size_t pos = 0;
    while (pos < n) {
        ssTables.emplace(itr, data, pos, Utility::SSTABLE_BOUND, SSTableId(dir, no++));
        ++size;
        byteCnt += ssTables.back().space();
    }
    save();
}

void LevelNonZero::clear() {
    while (!ssTables.empty()) {
        ssTables.back().remove();
        ssTables.pop_back();
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
    for (const SSTable &ssTable : ssTables) {
        uint64_t no = ssTable.number();
        ofs.write((char*) &no, sizeof(uint64_t));
    }
    ofs.close();
}