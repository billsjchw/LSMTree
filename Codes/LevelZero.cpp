#include "LevelZero.h"
#include "SSTableId.h"
#include "Utility.h"
#include <filesystem>
#include <fstream>

LevelZero::LevelZero(const std::string &dir): dir(dir) {
    if (!std::filesystem::exists(std::filesystem::path(dir))) {
        size = 0;
        std::filesystem::create_directory(std::filesystem::path(dir));
    } else {
        std::ifstream ifs(dir + "/index", std::ios::binary);
        ifs.read((char*) &size, sizeof(uint64_t));
        for (uint64_t i = 0; i < size; ++i) {
            uint64_t no;
            ifs.read((char*) &no, sizeof(uint64_t));
            ssTables.emplace_back(SSTableId(dir, no));
        }
        ifs.close();
    }
}

SearchResult LevelZero::search(uint64_t key) {
    for (uint64_t i = 1; i <= size; ++i) {
        SearchResult res = ssTables[i].search(key);
        if (res.success)
            return res;
    }
    return {false, ""};
}

void LevelZero::add(const SkipList &memTable, uint64_t &no) {
    ssTables.emplace_back(memTable, SSTableId(dir, no++));
    ++size;
}

std::vector<Entry> LevelZero::extract() {
    std::vector<std::vector<Entry>> inputs;
    for (const SSTable &ssTable : ssTables)
        inputs.emplace_back(ssTable.load());
    return Utility::compact(inputs);
}

void LevelZero::save() {
    std::ofstream ofs(dir + "/index", std::ios::binary);
    ofs.write((char*) &size, sizeof(uint64_t));
    for (uint64_t i = 0; i < size; ++i) {
        uint64_t no = ssTables[i].number();
        ofs.write((char*) &no, sizeof(uint64_t));
    }
    ofs.close();
}