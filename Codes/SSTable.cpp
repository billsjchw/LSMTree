#include "SSTable.h"
#include <fstream>
#include <sstream>
#include <filesystem>

SSTable::SSTable(const std::string &filename)
    : filename(filename) {
    std::ifstream ifs(filename, std::ios::binary);
    ifs.read((char*) &size, sizeof(uint64_t));
    for (uint64_t i = 0; i <= size; ++i) {
        uint64_t key, offset;
        ifs.read((char*) &key, sizeof(uint64_t));
        ifs.read((char*) &offset, sizeof(uint64_t));
        keys.push_back(key);
        offsets.push_back(offset);
    }
    ifs.close();
}

SSTable::SSTable(const SkipList &memTable, const std::string &filename)
    : filename(filename) {
    size = memTable.size();
    uint64_t offset = 0;
    std::ostringstream oss;
    SkipList::Iterator itr = memTable.iterator();
    while (itr.hasNext()) {
        Entry entry = itr.next();
        keys.push_back(entry.key);
        offsets.push_back(offset);
        offset += entry.value.length();
        oss << entry.value;
    }
    keys.push_back(0);
    offsets.push_back(offset);
    save(oss.str());
}

SSTable::SSTable(const std::vector<Entry> &data, uint64_t &pos, uint64_t bound, const std::string &filename)
    : filename(filename) {
    uint64_t n = data.size();
    size = 0;
    uint64_t offset = 0;
    std::ostringstream oss;
    while (pos < n && offset <= bound) {
        Entry entry = data[pos];
        keys.push_back(entry.key);
        offsets.push_back(offset);
        offset += entry.value.length();
        oss << entry.value;
        ++size;
        ++pos;
    }
    keys.push_back(0);
    offsets.push_back(offset);
    save(oss.str());
}

SearchResult SSTable::search(uint64_t key) {
    uint64_t left = 0;
    uint64_t right = size;
    while (right - left > 2) {
        uint64_t mid = left + (right - left) / 2;
        if (keys[mid] < key)
            left = mid;
        else if (keys[mid] > key)
            right = mid;
        else
            return {true, read(mid)};
    }
    if (keys[left] == key)
        return {true, read(left)};
    else if (keys[right - 1] == key)
        return {true, read(right - 1)};
    else
        return {false, ""};
}

std::vector<Entry> SSTable::load() {
    std::vector<Entry> ret;
    std::ifstream ifs(filename, std::ios::binary);
    ifs.seekg((2 * size + 3) * 8, std::ios::beg);
    for (uint64_t i = 0; i < size; ++i) {
        uint64_t key = keys[i];
        uint64_t len = offsets[i + 1] - offsets[i];
        char *buf = new char[len];
        ifs.read(buf, len);
        std::string value(buf);
        delete[] buf;
        ret.emplace_back(key, value);
    }
    ifs.close();
    return ret;
}

void SSTable::remove() {
    std::filesystem::remove(std::filesystem::path(filename));
}

void SSTable::save(const std::string &values) {
    std::ofstream ofs(filename, std::ios::binary);
    ofs.write((char*) &size, sizeof(uint64_t));
    for (uint64_t i = 0; i <= size; ++i) {
        ofs.write((char*) &keys[i], sizeof(uint64_t));
        ofs.write((char*) &offsets[i], sizeof(uint64_t));
    }
    ofs.write(values.c_str(), values.length());
    ofs.close();
}

std::string SSTable::read(uint64_t pos) {
    std::ifstream ifs(filename);
    uint64_t len = offsets[pos + 1] - offsets[pos];
    char *buf = new char[len];
    ifs.seekg((2 * size + 3) * 8 + offsets[pos], std::ios::beg);
    ifs.read(buf, len);
    std::string value(buf);
    delete[] buf;
    ifs.close();
    return value;
}