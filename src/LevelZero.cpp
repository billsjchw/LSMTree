#include "LevelZero.h"
#include "SSTableId.h"
#include "Util.h"
#include <filesystem>
#include <fstream>

LevelZero::LevelZero(const std::string &dir): dir(dir) {
    if (!std::filesystem::exists(std::filesystem::path(dir))) {
        std::filesystem::create_directories(std::filesystem::path(dir));
        size = 0;
        byteCnt = 0;
        save();
    } else {
        std::ifstream ifs(dir + "/index", std::ios::binary);
        ifs.read((char*) &size, sizeof(uint64_t));
        ifs.read((char*) &byteCnt, sizeof(uint64_t));
        for (uint64_t i = 0; i < size; ++i) {
            uint64_t no;
            ifs.read((char*) &no, sizeof(uint64_t));
            ssts.emplace_back(SSTableId(dir, no));
        }
        ifs.close();
    }
}

SearchResult LevelZero::search(uint64_t key) const {
    for (uint64_t i = 1; i <= size; ++i) {
        SearchResult res = ssts[size - i].search(key);
        if (res.success)
            return res;
    }
    return {false, ""};
}

void LevelZero::add(const SkipList &mem, uint64_t &no) {
    ssts.emplace_back(mem, SSTableId(dir, no++));
    ++size;
    byteCnt += mem.space();
    save();
}

std::vector<Entry> LevelZero::extract() {
    std::vector<std::vector<Entry>> inputs;
    for (const SSTable &sst : ssts) {
        inputs.emplace_back(sst.load());
        sst.remove();
    }
    size = 0;
    byteCnt = 0;
    ssts.clear();
    save();
    return Util::compact(inputs);
}

void LevelZero::clear() {
    while (!ssts.empty()) {
        ssts.back().remove();
        ssts.pop_back();
    }
    size = 0;
    byteCnt = 0;
    save();
}

uint64_t LevelZero::space() const {
    return byteCnt;
}

void LevelZero::save() const {
    std::ofstream ofs(dir + "/index", std::ios::binary);
    ofs.write((char*) &size, sizeof(uint64_t));
    ofs.write((char*) &byteCnt, sizeof(uint64_t));
    for (const SSTable &sst : ssts) {
        uint64_t no = sst.number();
        ofs.write((char*) &no, sizeof(uint64_t));
    }
    ofs.close();
}