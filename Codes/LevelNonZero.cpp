#include "LevelNonZero.h"
#include "Utility.h"
#include "SSTableId.h"
#include <filesystem>
#include <fstream>

LevelNonZero::LevelNonZero(const std::string &dir): dir(dir) {
    if (!std::filesystem::exists(std::filesystem::path(dir))) {
        size = 0;
        lastKey = 0;
        std::filesystem::create_directories(std::filesystem::path(dir));
    } else {
        std::ifstream ifs(dir + "/index", std::ios::binary);
        ifs.read((char*) &size, sizeof(uint64_t));
        ifs.read((char*) &lastKey, sizeof(uint64_t));
        for (uint64_t i = 0; i < size; ++i) {
            uint64_t no;
            ifs.read((char*) &no, sizeof(uint64_t));
            ssTables.emplace_back(SSTableId(dir, no));
        }
        ifs.close();
    }
}

LevelNonZero::~LevelNonZero() {
    std::ofstream ofs(dir + "/index", std::ios::binary);
    ofs.write((char*) &size, sizeof(uint64_t));
    ofs.write((char*) &lastKey, sizeof(uint64_t));
    for (const SSTable &ssTable : ssTables) {
        uint64_t no = ssTable.number();
        ofs.write((char*) &no, sizeof(uint64_t));
    }
    ofs.close();
}

SearchResult LevelNonZero::search(uint64_t key) {
    for (const SSTable &ssTable : ssTables) {
        SearchResult res = ssTable.search(key);
        if (res.success)
            return res;
    }
    return {false, ""};
}

std::vector<Entry> LevelNonZero::extract() {
    std::list<SSTable>::iterator itr = ssTables.begin();
    while (itr != ssTables.end() && itr->upper() <= lastKey)
        ++itr;
    if (itr == ssTables.end())
        itr = ssTables.begin();
    lastKey = itr->upper();
    std::vector<Entry> ret = itr->load();
    itr->remove();
    ssTables.erase(itr);
    return ret; 
}

void LevelNonZero::merge(const std::vector<Entry> &lData, uint64_t &no) {
    uint64_t lo = lData[0].key;
    uint64_t hi = lData.back().key;
    std::vector<Entry> eData;
    std::list<SSTable>::iterator itr = ssTables.begin();
    while (itr != ssTables.end() && itr->upper() < lo)
        ++itr;
    while (itr != ssTables.end() && itr->lower() <= hi) {
        for (const Entry &entry : itr->load())
            eData.emplace_back(entry);
        itr->remove();
        itr = ssTables.erase(itr);
        --size;
    }
    std::vector<Entry> data = Utility::compact({eData, lData});
    size_t n = data.size();
    size_t pos = 0;
    while (pos < n) {
        ssTables.emplace(itr, data, pos, Utility::BOUND, SSTableId(dir, no++));
        ++size;
    }
}